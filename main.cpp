#include "declarations.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Enter name of input file";
    }
    cout << "Reading input file: " << argv[1] << endl;
    cout << "-----------------------------------" << endl;
    string inputfile = argv[1];
    prm.load(inputfile);
    cout << "Attachment probability: " << 1.0 - exp(-prm.k_react * prm.dt) << endl;
    cout << "-----------------------------------" << endl;

    // initialize filament(s)
    vd f1start, f1head;
    f1start.resize(3);
    f1head.resize(3);
    f1start = {0.0, 0.0, 0.0};
    f1head = {0.0, 0.0, 1.0};

    filament f1(prm.length, prm.radius, prm.mass, f1start, f1head);

    report_diffusion_constants(f1);

    vd initial_CoM;
    initial_CoM = f1.get_CoM();

    ofstream dump;
    dump.open("dump/dump.dat");
    dump_positions(f1, dump, "filament1", "m1");

    ofstream CoM_disp;
    CoM_disp.open("data/CoM_displacement.dat");
    CoM_disp << "#t/tau"
             << "\t"
             << "dx"
             << "\t"
             << "dy"
             << "\t"
             << "dz"
             << "\t"
             << "ds^2"
             << "\t"
             << endl;

    time_info();

    int t_iter = 0;
    while (t_iter < prm.iterations && f1.is_attached == false)
    {
        perform_step(f1);

        if (t_iter % prm.steps_to_skip == 0)
        {
            dump_positions(f1, dump, "filament1", "m1");

            CoM_displacement_sq(f1, initial_CoM, t_iter, CoM_disp);
        }

        t_iter++;
    }

    dump.close();
    CoM_disp.close();

    // wasted_steps_stats();

    cout << "Simulation complete." << endl;
    cout << "-----------------------------------" << endl;

    cout << "Number of times bounced: " << times_bounced << endl;
    if (f1.is_attached == true)
    {
        double attach_time = (t_iter * prm.dt * get_tau()) / milli;
        cout << "Filament attached at t = "
             << attach_time
             << " ms." << endl;
    }
    else
    {
        cout << "Filament not attached." << endl;
    }
}