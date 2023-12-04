#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <complex>
#include "parameters.h"
#include "monomer.h"
#include <iomanip>
#include <cassert>
#include <string>
#include <sstream>
#include <cassert>
#include "vectormaths.h"
#include <random>
#include "ftsz.h"
#include "filament.h"

#define PI acos(-1.0)

#define sigma_scale 1.0
#define epsilon_scale 1.0
#define nano 1E-9
#define micro 1E-6
#define milli 1E-3

using namespace std;

parameters prm;

// int main(int argc, char *argv[])
// {
//     if (argc != 2)
//     {
//         std::cerr << "Enter name of input file";
//     }
//     string inputfile = argv[1];
//     prm.load(inputfile);

//     std::mt19937 gen(prm.seed);
//     std::uniform_real_distribution<> dis(0, 1); // uniform distribution between 0 and 1
//     ofstream testdump;
//     testdump.open("test.dump");
//     for (int n = 0; n < 10000000; ++n)
//     {
//         testdump << dis(gen) << endl;
//     }
//     testdump.close();
// }

int main()
{
    filament f1(5, 0.2, 2.0);

    for (int i = 0; i < f1.size(); i++)
    {
        f1.monomers[i].radius = 1.3 * i;
        cout << "i = " << i
             << " radius = " << f1.monomers[i].radius
             << " mass = " << f1.monomers[i].mass << endl;
        cout
            << " force = " << printvec(f1.monomers[i].force)
            << endl;
    }
    cout << "Total mass = " << f1.totalmass() << endl;

    f1.monomers.erase(f1.monomers.begin() + 1);
    f1.monomers[3].mass = 1000.232;
    f1.monomers[2].set_force(0.1, 3.4, -21.003);

    cout << "AFTER ERASING AND CHANGING" << endl;

    for (int i = 0; i < f1.size(); i++)
    {
        cout << "i = " << i
             << " radius = " << f1.monomers[i].radius
             << " mass = " << f1.monomers[i].mass << endl;
        cout
            << " force = " << printvec(f1.monomers[i].force)
            << endl;
    }

    cout << "Total mass = " << f1.totalmass() << endl;
}