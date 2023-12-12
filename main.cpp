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

#define PI acos(-1.0)

#define sigma_scale 1.0
#define epsilon_scale 1.0
#define nano 1E-9
#define micro 1E-6
#define milli 1E-3

using namespace std;

parameters prm;
std::mt19937 gen(prm.seed);
std::normal_distribution<> dis_normal(0.0, 1.0);

double brownian_translate(double, double);

double brownian_translate(double F_cons_component, double D)
{
    double c1, c2;
    double W;
    W = dis_normal(gen);
    c1 = D / (prm.kB * prm.T);
    c2 = sqrt(2.0 * D * prm.dt);
    return c1 * F_cons_component * prm.dt + c2 * W;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Enter name of input file";
    }
    string inputfile = argv[1];
    prm.load(inputfile);

    // initialize filament(s)
    vd f1start, f1head;
    f1start.resize(3);
    f1head.resize(3);
    f1start = {0.0, 0.0, 0.0};
    f1head = {-1.0, 0.0, 0.0};

    filament f1(prm.length, prm.radius, prm.mass, f1start, f1head);

    ofstream dump;
    dump.open("dump.dat");

    // dump initial position
    dump << f1.length() << endl;
    dump << "filament1" << endl;
    for (int i = 0; i < f1.length(); i++)
    {
        dump << "1"
             << " "
             << f1.monomers[i].pos[0] << " "
             << f1.monomers[i].pos[1] << " "
             << f1.monomers[i].pos[2] << " "
             << endl;
    }

    double dx, dy, dz;
    vd ds_local, ds_global;

    vd x, y, z, X, Y, Z;
    X = {1.0, 0.0, 0.0};
    Y = {0.0, 1.0, 0.0};
    Z = {0.0, 0.0, 1.0};

    for (int t_iter = 0; t_iter < prm.iterations; t_iter++)
    {

        for (int i = 0; i < f1.length(); i++)
        {
            f1.monomers[i].reset_force();
        }

        dx = brownian_translate(0.0, prm.D_perp);
        dy = brownian_translate(0.0, prm.D_perp);
        dz = brownian_translate(0.0, prm.D_par);

        ds_local = {dx, dy, dz};

        z = f1.heading;
        x = f1.face();
        y = cross3D(z, x) * (1.0 * norm((z, x)));

        ds_global = convertbasis(ds_local, x, y, z, X, Y, Z);

        f1.displace_filament(ds_global);

        if (t_iter % prm.steps_to_skip == 0)
        {
            dump << f1.length() << endl;
            dump << "filament1" << endl;
            for (int i = 0; i < f1.length(); i++)
            {
                dump << "1"
                     << " "
                     << f1.monomers[i].pos[0] << " "
                     << f1.monomers[i].pos[1] << " "
                     << f1.monomers[i].pos[2] << " "
                     << endl;
            }
        }
    }

    dump.close();
}

// int main(int argc, char *argv[])
// {
//     if (argc != 2)
//     {
//         std::cerr << "Enter name of input file";
//     }
//     string inputfile = argv[1];
//     prm.load(inputfile);

//     // brownian_engine be1(1.0, 1.0, 1.0, 1.0, gen, dis);
//     // brownian_engine be2(1.0, 1.0, 1.0, 1.0, gen, dis);

//     // std::uniform_real_distribution<> dis(0, 1);
//     ofstream testdump;
//     testdump.open("test.dump");
//     for (int n = 0; n < 100000; ++n)
//     {
//         // testdump << dis(gen) << endl;
//         // testdump << be1.translation(0.0) << " "
//         //          << be2.translation(0.0) << endl;
//         // testdump << be1.normal() << " "
//         //          << be2.normal() << endl;
//         // double r1, r2;
//         // r1 = dis(gen);
//         // r2 = dis(gen);
//         testdump << brownian_translate(0.0, 1.0) << " "
//         << brownian_translate(0.0, 1.0) << endl;
//     }
//     testdump.close();
// }

// int main()
// {
//     vd f1start, f1head, ds;
//     vd old_CoM;
//     f1start.resize(3);
//     f1head.resize(3);
//     ds.resize(3);

//     // f1start = {1.3, -2.5, 3.0};
//     f1start = {0.0, 0.0, 0.0};

//     f1head = {1.2, -5.3, 1.0};
//     // f1head = {0.0, 0.0, 1.0};

//     // ds = {0.0, 1.3, 1.1};
//     ds = {1.0, 0.0, 0.0};

//     // f1face = {1.0, 0.0, 0.0};

//     filament f1(5, 0.5, 1.0, f1start, f1head);

//     for (int i = 0; i < f1.length(); i++)
//     {
//         cout << i << " " << printvec(f1.monomers[i].pos) << endl;
//     }
//     cout << "Monomers = " << f1.length() << endl;
//     cout << "Contour length = " << f1.contour_length() << endl;
//     cout << "CoM = " << printvec(f1.get_CoM()) << endl;
//     cout << "heading = " << printvec(f1.heading) << endl;
//     cout << "face = " << printvec(f1.face()) << endl;
//     cout << "|face| = " << norm(f1.face()) << endl;
//     cout << "h.f = " << dot(f1.heading, f1.face()) << endl;
//     old_CoM = f1.get_CoM();

//     // cout << "=======TRANSFORMING=======" << endl;
//     // f1.add_at_beginning(0.5, 1.0);
//     // f1.remove_at_end();
//     // f1.displace_filament(ds);
//     // f1.add_at_beginning(0.5, 1.0);
//     // f1.remove_at_end();
//     // f1.add_at_beginning(0.5, 1.0);

//     // for (int i = 0; i < f1.length(); i++)
//     // {
//     //     cout << i << " " << printvec(f1.monomers[i].pos) << endl;
//     // }
//     // cout << "Monomers = " << f1.length() << endl;
//     // cout << "Contour length = " << f1.contour_length() << endl;
//     // cout << "CoM = " << printvec(f1.get_CoM()) << endl;
//     // cout << "==================" << endl;

//     // vd a, b;
//     // a = {0.0, 0.0, 1.0};
//     // b = {1.0, 0.0, 0.0};
//     // cout << "a = " << printvec(a) << endl
//     //      << "b = " << printvec(b) << endl;
//     // cout << "Cross(a, b) = " << endl
//     //      << printvec(cross3D(a, b)) << endl;

//     vd x, y, z, X, Y, Z, exp_x, exp_y, exp_z;

//     X = {1.0, 0.0, 0.0};
//     Y = {0.0, 1.0, 0.0};
//     Z = {0.0, 0.0, 1.0};

//     z = f1.heading;
//     x = f1.face();
//     y = (cross3D(z, x)) * (1.0 / norm(cross3D(z, x)));

//     // exp_x = (dot(X, x)) * X + (dot(Y, x)) * Y + (dot(Z, x)) * Z;
//     // exp_y = (dot(X, y)) * X + (dot(Y, y)) * Y + (dot(Z, y)) * Z;
//     // exp_z = (dot(X, z)) * X + (dot(Y, z)) * Y + (dot(Z, z)) * Z;

//     bool print_verifications = 0;

//     if (print_verifications)
//     {
//         cout << "X = " << printvec(X) << endl;
//         cout << "Y = " << printvec(Y) << endl;
//         cout << "Z = " << printvec(Z) << endl;

//         cout << "x = " << printvec(x) << endl;
//         cout << "y = " << printvec(y) << endl;
//         cout << "z = " << printvec(z) << endl;

//         cout << "x.y = " << dot(x, y) << endl;
//         cout << "y.z = " << dot(z, y) << endl;
//         cout << "z.x = " << dot(x, z) << endl;
//         cout << "x.x = " << dot(x, x) << endl;
//         cout << "y.y = " << dot(y, y) << endl;
//         cout << "z.z = " << dot(z, z) << endl;

//         cout << "Converting unit vectors to global coordinates" << endl;

//         cout << "x.y = " << dot(exp_x, exp_y) << endl;
//         cout << "y.z = " << dot(exp_z, exp_y) << endl;
//         cout << "z.x = " << dot(exp_x, exp_z) << endl;
//         cout << "x.x = " << dot(exp_x, exp_x) << endl;
//         cout << "y.y = " << dot(exp_y, exp_y) << endl;
//         cout << "z.z = " << dot(exp_z, exp_z) << endl;
//     }

//     vd local_displacement, global_displacement, global_by_function;

//     local_displacement = {1.0, -2.03, 0.1};
//     cout << "local displacement = " << printvec(local_displacement) << endl;

//     // global_displacement = local_displacement[0] * exp_x +
//     //                       local_displacement[1] * exp_y +
//     //                       local_displacement[2] * exp_z;

//     global_displacement = convertbasis(local_displacement, x, y, z, X, Y, Z);

//     cout << "global displacement = " << printvec(global_displacement) << endl;
//     // cout << "global displacement fn =" << printvec(global_by_function) << endl;

//     cout << "=======TRANSFORMING=======" << endl;
//     f1.displace_filament(global_displacement);

//     for (int i = 0; i < f1.length(); i++)
//     {
//         cout << i << " " << printvec(f1.monomers[i].pos) << endl;
//     }
//     cout << "Monomers = " << f1.length() << endl;
//     cout << "Contour length = " << f1.contour_length() << endl;
//     cout << "CoM = " << printvec(f1.get_CoM()) << endl;
//     cout << "heading = " << printvec(f1.heading) << endl;
//     cout << "face = " << printvec(f1.face()) << endl;
//     cout << "|face| = " << norm(f1.face()) << endl;
//     cout << "h.f = " << dot(f1.heading, f1.face()) << endl;
//     cout << "Displacement of CoM = "
//          << printvec(f1.get_CoM() - old_CoM) << endl;
//     cout << "Distance in local = " << norm(local_displacement) << endl;
//     cout << "Distance in global = " << norm(global_displacement) << endl;
//     cout << "Distance by CoM = " << norm(f1.get_CoM() - old_CoM) << endl;
// }