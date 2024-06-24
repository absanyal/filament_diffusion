#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cassert>
#include <string>

#include "common_objects.h"
#include "filament.h"
#include "vectormaths.h"
#include "quaternion.h"
#include "parameters.h"

double dh = 50.0;
double dphi = 0.1;

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

int cell_particle_count()
{
     double rho = prm.cell_radius;
     double h_lo = prm.xlo;
     double h_hi = prm.xhi;

     double h;
     double phi;

     int h_points = int((h_hi - h_lo) / dh);
     int phi_points = int(2 * M_PI / dphi);

     return h_points * phi_points;
}

void dump_cell(ofstream &dump)
{
     double rho = prm.cell_radius;
     double h_lo = prm.xlo;
     double h_hi = prm.xhi;

     double h;
     double phi;

     int h_points = int((h_hi - h_lo) / dh);
     int phi_points = int(2 * M_PI / dphi);

     for (int hi = 0; hi < h_points; hi++)
     {
          h = h_lo + hi * dh;
          for (int phi_i = 0; phi_i < phi_points; phi_i++)
          {
               phi = phi_i * dphi;
               dump << "cell "
                    << h << " "
                    << rho * cos(phi) << " "
                    << rho * sin(phi) << " "
                    << "1.0"
                    << endl;
          }
     }
}

void dump_positions(filament f, ofstream &dumpfile, string filament_label, string monomer_label, bool box = true, bool cell = true)

{

     int box_count = 7 * box;

     int cell_count;
     if (cell == true)
     {
          cell_count = cell_particle_count() * cell;
     }
     else
     {
          cell_count = 0;
     }

     dumpfile << f.length() + box_count + cell_count << endl;

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

     if (cell == true)
     {
          dump_cell(dumpfile);
     }
}
