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
    vd f1start, f1head, ds;
    f1start.resize(3);
    f1head.resize(3);
    ds.resize(3);

    f1start = {0.0, 0.0, 0.0};

    f1head = {-1.0, 0.0, 0.0};

    ds = {0.0, 1.3, 1.1};

    filament f1(5, 0.5, 1.0, f1start, f1head);

    for (int i = 0; i < f1.length(); i++)
    {
        cout << i << " " << printvec(f1.monomers[i].pos) << endl;
    }
    cout << "Monomers = " << f1.length() << endl;
    cout << "Contour length = " << f1.contour_length() << endl;
    cout << "CoM = " << printvec(f1.get_CoM()) << endl;

    cout << "=======TRANSFORMING=======" << endl;
    // f1.displace_filament(ds);
    f1.add_at_beginning(0.5, 1.0);
    f1.remove_at_end();


    for (int i = 0; i < f1.length(); i++)
    {
        cout << i << " " << printvec(f1.monomers[i].pos) << endl;
    }
    cout << "Monomers = " << f1.length() << endl;
    cout << "Contour length = " << f1.contour_length() << endl;
    cout << "CoM = " << printvec(f1.get_CoM()) << endl;
}