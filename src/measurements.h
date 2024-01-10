#pragma once

#include "common_objects.h"
#include "vectormaths.h"
#include "parameters.h"
#include "filament.h"

void wasted_steps_stats()
{
     cout << "Performed steps = " << prm.iterations + wasted_steps << endl;
     cout << "Wasted steps = " << wasted_steps << endl;
     cout << "% of steps wasted = " << (100.0 * wasted_steps) / (1.0 * (prm.iterations + wasted_steps)) << endl;
}

void CoM_displacement_sq(filament f, vd init_CoM, int iter_num, ofstream &dump)
{
     vd ds, current_CoM;
     current_CoM = f.get_CoM();
     ds = current_CoM - init_CoM;
     dump << (1.0 * iter_num * prm.dt) << "\t"
          << ds[0] << "\t"
          << ds[1] << "\t"
          << ds[2] << "\t"
          << dot(ds, ds) << "\t"
          << endl;
}