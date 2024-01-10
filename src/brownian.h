#pragma once

#include "common_objects.h"
#include "parameters.h"
#include "vectormaths.h"
#include "filament.h"
#include "quaternion.h"
#include "cylinder.h"

double brownian_translate(double F_cons_component, double D)
{
     double c1, c2;
     double W;
     if (!rng_seeded){
          gen.seed(prm.seed);
          rng_seeded = true;
     }
     std::normal_distribution<> dis_normal(0.0, 1.0);
     W = dis_normal(gen);
     c1 = D / (prm.kB * prm.T);
     c2 = sqrt(2.0 * D * prm.dt);
     return c1 * F_cons_component * prm.dt + c2 * W;
}

vd global_brownian_displacement(filament f)
{
     vd result;
     double dx, dy, dz;
     vd x, y, z;

     vd ds_local;

     dx = brownian_translate(0.0, prm.D_perp);
     dy = brownian_translate(0.0, prm.D_perp);
     dz = brownian_translate(0.0, prm.D_par);

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

     d_theta = brownian_translate(0.0, prm.D_rot);

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

     while (end1_in == false || end2_in == false)
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

          if (end1_in == false || end2_in == false)
          {
               f.rotate_filament(-d_theta_y, "y");
               f.rotate_filament(-d_theta_x, "x");
               f.displace_filament(-ds_global);
               wasted_steps += 1;
          }
     }
}