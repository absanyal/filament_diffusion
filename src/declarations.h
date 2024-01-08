#pragma once

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <complex>
#include <iomanip>
#include <cassert>
#include <string>
#include <sstream>
#include <cassert>
#include <random>

#include "vectormaths.h"
#include "parameters.h"
#include "monomer.h"
#include "ftsz.h"
#include "filament.h"
#include "quaternion.h"

#define PI acos(-1.0)

#define sigma_scale 1.0
#define epsilon_scale 1.0
#define nano 1E-9
#define micro 1E-6
#define milli 1E-3

parameters prm;
std::mt19937 gen(prm.seed);
std::normal_distribution<> dis_normal(0.0, 1.0);

double axis_scale = 20.0;

vd X = {1.0, 0.0, 0.0};
vd Y = {0.0, 1.0, 0.0};
vd Z = {0.0, 0.0, 1.0};

int wasted_steps = 0;

bool check_inside_cylinder(double R, vd rA, vd rB, vd rP)
{
     vd e, m, rQ;
     double d;
     double wA, wB;
     double rArA, rArB, rBrB, rQrA, rQrB;

     bool inside = true;

     e = rA - rB;
     m = cross3D(rA, rB);
     d = norm(cross3D(e, rP - rA)) / norm(e);

     rQ = rP + ((cross3D(e, m + cross3D(e, rP))) / (pow(norm(e), 2)));

     rArA = dot(rA, rA);
     rArB = dot(rA, rB);
     rBrB = dot(rB, rB);
     rQrA = dot(rQ, rA);
     rQrB = dot(rQ, rB);

     wA = -(rArB - rBrB - rQrA - rBrB * rQrA + rQrB + rArB * rQrB) / (rArA - 2.0 * rArB - pow(rArB, 2) + rBrB + rArA * rBrB);

     wB = -(rArA - rArB - rQrA - rArB * rQrA + rQrB + rArA * rQrB) /
          (-rArA + 2.0 * rArB + pow(rArB, 2) - rBrB - rArA * rBrB);

     if (d > R || wA < 0.0 || wA > 1.0 || wB < 0.0 || wB > 1.0)
     {
          inside = false;
     }

     return inside;
}

double brownian_translate(double F_cons_component, double D)
{
     double c1, c2;
     double W;
     W = dis_normal(gen);
     c1 = D / (prm.kB * prm.T);
     c2 = sqrt(2.0 * D * prm.dt);
     return c1 * F_cons_component * prm.dt + c2 * W;
}

void dump_box(ostream &dump)
{
     vd rxlo, rxhi, rylo, ryhi, rzlo, rzhi;
     rxlo = {prm.xlo, 0.0, 0.0};
     rxhi = {prm.xhi, 0.0, 0.0};
     rylo = {0.0, -prm.cell_radius, 0.0};
     ryhi = {0.0, prm.cell_radius, 0.0};
     rzlo = {0.0, 0.0, -prm.cell_radius};
     rzhi = {0.0, 0.0, prm.cell_radius};

     dump << "box 0.0 0.0 0.0 0.5" << endl;

     dump << "box "
          << rxlo[0] << " " << rxlo[1] << " " << rxlo[2] << " 0.5" << endl;
     dump << "box "
          << rxhi[0] << " " << rxhi[1] << " " << rxhi[2] << " 0.5" << endl;

     dump << "box "
          << rylo[0] << " " << rylo[1] << " " << rylo[2] << " 0.5" << endl;
     dump << "box "
          << ryhi[0] << " " << ryhi[1] << " " << ryhi[2] << " 0.5" << endl;

     dump << "box "
          << rzlo[0] << " " << rzlo[1] << " " << rzlo[2] << " 0.5" << endl;
     dump << "box "
          << rzhi[0] << " " << rzhi[1] << " " << rzhi[2] << " 0.5" << endl;
}

void dump_positions(filament f, ofstream &dumpfile, string filament_label, string monomer_label, bool box = true)
{
     if (box == true)
     {
          dumpfile << f.length() + 7 << endl;
     }
     else
     {
          dumpfile << f.length() << endl;
     }
     dumpfile << filament_label << endl;
     for (int i = 0; i < f.length(); i++)
     {
          dumpfile << monomer_label
                   << " "
                   << f.monomers[i].pos[0] << " "
                   << f.monomers[i].pos[1] << " "
                   << f.monomers[i].pos[2] << " "
                   << f.monomers[i].radius << " "
                   << endl;
     }

     if (box == true)
     {
          dump_box(dumpfile);
     }
}

vd global_brownian_displacement(filament f)
{
     vd result;
     double dx, dy, dz;
     vd x, y, z;

     vd ds_local;

     bool proposed_ends_are_inside = false;

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

          end1_in = check_inside_cylinder(prm.cell_radius, rA, rB, end1);
          end2_in = check_inside_cylinder(prm.cell_radius, rA, rB, end2);

          if (end1_in == false || end2_in == false)
          {
               f.rotate_filament(-d_theta_y, "y");
               f.rotate_filament(-d_theta_x, "x");
               f.displace_filament(-ds_global);
               wasted_steps += 1;
          }
     }
}

void wasted_steps_stats()
{
     cout << "Performed steps = " << prm.iterations + wasted_steps << endl;
     cout << "Wasted steps = " << wasted_steps << endl;
     cout << "% of steps wasted = " << (100.0 * wasted_steps) / (1.0 * (prm.iterations + wasted_steps)) << endl;
}
