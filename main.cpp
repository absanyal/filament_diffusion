#include "declarations.h"

using namespace std;

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
    f1head = {0.0, 0.0, 1.0};

    filament f1(prm.length, prm.radius, prm.mass, f1start, f1head);

    ofstream dump;
    dump.open("dump.dat");

    dump_positions(f1, dump, "filament1", "m1");
    dump_box(dump);

    for (int t_iter = 0; t_iter < prm.iterations; t_iter++)
    {
        perform_step(f1);

        if (t_iter % prm.steps_to_skip == 0)
        {
            dump_positions(f1, dump, "filament1", "m1");
            dump_box(dump);
        }
    }

    dump.close();
}