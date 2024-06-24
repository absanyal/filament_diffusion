#pragma once

#include "common_objects.h"
#include "parameters.h"
#include "vectormaths.h"
#include "filament.h"
#include "quaternion.h"
#include "cylinder.h"

double D_0()
{
     double d;
     d = 2.0 * prm.radius * sigma0;
     return (kB0 * T0) / (6.0 * PI * (eta0 * prm.eta) * (d / 2.0));
}

double D_perp(double p)
{
     double nu_perp;
     double d;
     d = 2.0 * prm.radius * sigma0;
     double eta;
     eta = prm.eta * eta0;
     nu_perp = 0.839 + (0.185 / p) + (0.233 / (p * p));
     return (kB0 * (prm.T) * (nu_perp + log(p))) / (4.0 * d * p * PI * eta);
}

double D_par(double p)
{
     double nu_par;
     double d;
     d = 2.0 * prm.radius * sigma0;
     double eta;
     eta = prm.eta * eta0;
     nu_par = -0.207 + (0.980 / p) - (0.133 / (p * p));
     return (kB0 * (prm.T) * (nu_par + log(p))) / (2.0 * d * p * PI * eta);
}

double D_rot_0()
{
     return D_0() / (sigma0 * sigma0);
}

double D_rot(double p)
{
     double delta_rot;
     double d;
     d = 2.0 * prm.radius * sigma0;
     double eta;
     eta = prm.eta * eta0;
     delta_rot = -0.622 + 0.917 / p - 0.050 / (p * p);
     return 3.0 * kB0 * prm.T * (delta_rot + log(p)) / (pow(d, 3) * pow(p, 3) * PI * eta);
}

bool roll_for_attachment()
{
     if (!rng_seeded)
     {
          gen.seed(prm.seed);
          rng_seeded = true;
     }
     double r;
     std::uniform_real_distribution<double> dis_uniform(0.0, 1.0);
     r = dis_uniform(gen);

     double p;
     // p = 0.005;
     p = 0.1;

     if (r < p)
     {
          return true;
     }
     else
     {
          return false;
     }
}

double brownian_translate(double F_cons_component, double D)
{
     double c1, c2;
     double W;
     if (!rng_seeded)
     {
          gen.seed(prm.seed);
          rng_seeded = true;
     }
     std::normal_distribution<> dis_normal(0.0, 1.0);

     double kB = 1.0;
     double T;
     T = prm.T / T0;

     W = dis_normal(gen);
     c1 = D / (kB * T);
     c2 = sqrt(2.0 * D * prm.dt);
     return c1 * F_cons_component * prm.dt + c2 * W;
}

vd global_brownian_displacement(filament f)
{
     vd result;
     double dx, dy, dz;
     vd x, y, z;

     vd ds_local;

     double p;
     p = f.contour_length() / f.monomers[0].radius;

     double val_D_perp, val_D_par;

     if (prm.manual_diffusion_constants == 1)
     {
          val_D_perp = prm.D_perp / D_0();
          val_D_par = prm.D_par / D_0();
     }
     else
     {
          val_D_par = D_par(p) / D_0();
          val_D_perp = D_perp(p) / D_0();
     }

     dx = brownian_translate(0.0, val_D_perp);
     dy = brownian_translate(0.0, val_D_perp);
     dz = brownian_translate(0.0, val_D_par);

     ds_local = {dx, dy, dz};

     z = f.heading;
     x = f.face();
     y = cross3D(z, x) * (1.0 * norm((z, x)));

     result = convertbasis(ds_local, x, y, z, X, Y, Z);

     return result;
}

double global_brownian_angle(filament f)
{
     double result;
     double d_theta;

     double p;
     p = f.contour_length() / f.monomers[0].radius;

     double val_D_rot;

     if (prm.manual_diffusion_constants == 1)
     {
          val_D_rot = prm.D_rot / D_rot_0();
     }
     else
     {
          val_D_rot = D_rot(p) / D_rot_0();
     }

     d_theta = brownian_translate(0.0, val_D_rot);

     result = d_theta;

     return result;
}

void perform_step(filament &f)
{
     vd ds_global;
     double d_theta_x, d_theta_y;

     bool end1_in, end2_in;
     end1_in = false;
     end2_in = false;

     vd end1, end2;

     vd rA, rB;

     rA = {prm.xlo, 0.0, 0.0};
     rB = {prm.xhi, 0.0, 0.0};

     while ((end1_in == false || end2_in == false) && f.is_attached == false)
     {
          ds_global = global_brownian_displacement(f);
          d_theta_x = global_brownian_angle(f);
          d_theta_y = global_brownian_angle(f);

          f.displace_filament(ds_global);
          f.rotate_filament(d_theta_x, "x");
          f.rotate_filament(d_theta_y, "y");

          end1 = f.monomers[0].pos;
          end2 = f.monomers[f.length() - 1].pos;

          if (prm.wall_collisions == 1)
          {
               end1_in = check_inside_cylinder(prm.cell_radius, rA, rB, end1);
               end2_in = check_inside_cylinder(prm.cell_radius, rA, rB, end2);
          }
          else
          {
               end1_in = true;
               end2_in = true;
          }

          if ((end1_in == false || end2_in == false) && f.is_attached == false)
          {
               f.is_attached = roll_for_attachment();
          }

          if ((end1_in == false || end2_in == false) && f.is_attached == false)
          {

               f.rotate_filament(-d_theta_y, "y");
               f.rotate_filament(-d_theta_x, "x");
               f.displace_filament(-ds_global);
               wasted_steps += 1;
          }
     }
}