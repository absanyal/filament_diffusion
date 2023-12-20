#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <cassert>

using std::ifstream;
using std::string;

using namespace std;

typedef std::complex<double> cd;

class parameters
{
    // private:
    //     /* data */
public:
    int seed;

    int length;
    double radius, mass;

    int iterations;
    float dt;

    float D_par, D_perp, D_rot;

    float kB, T;

    double xhi, xlo, cell_radius;

    int steps_to_skip;

    double matchstring(string, string);
    string matchstring2(string, string);
    void load(string);
};

double parameters::matchstring(string file, string match)
{
    string test;
    string line;
    ifstream readFile(file);
    double amount;
    bool pass = false;
    while (std::getline(readFile, line))
    {
        std::istringstream iss(line);
        if (std::getline(iss, test, '=') && pass == false)
        {
            // ---------------------------------
            if (iss >> amount && test == match)
            {
                // cout << amount << endl;
                pass = true;
            }
            else
            {
                pass = false;
            }
            // ---------------------------------
            if (pass)
                break;
        }
    }
    if (pass == false)
    {
        // string errorout = match;
        // errorout += "= argument is missing in the input file!";
        // throw std::invalid_argument(errorout);
        cout << match << "= argument is missing in the input file!";
    }
    cout << match << " = " << amount << endl;
    return amount;
}

string parameters::matchstring2(string file, string match)
{

    string line;
    ifstream readFile(file);
    string amount;
    int offset;

    if (readFile.is_open())
    {
        while (!readFile.eof())
        {
            getline(readFile, line);

            if ((offset = line.find(match, 0)) != string::npos)
            {
                amount = line.substr(offset + match.length() + 1);
            }
        }
        readFile.close();
    }
    else
    {
        cout << "Parameter missing" << endl;
    }

    cout << match << " = " << amount << endl;
    return amount;
}

void parameters::load(string inputfile)
{
    seed = matchstring(inputfile, "seed");

    length = matchstring(inputfile, "length");
    radius = matchstring(inputfile, "radius");
    mass = matchstring(inputfile, "mass");

    iterations = matchstring(inputfile, "iterations");
    dt = matchstring(inputfile, "dt");

    D_par = matchstring(inputfile, "D_par");
    D_perp = matchstring(inputfile, "D_perp");
    D_rot = matchstring(inputfile, "D_rot");

    xlo = matchstring(inputfile, "xlo");
    xhi = matchstring(inputfile, "xhi");
    cell_radius = matchstring(inputfile, "cell_radius");

    kB = matchstring(inputfile, "kB");
    T = matchstring(inputfile, "T");

    steps_to_skip = matchstring(inputfile, "steps_to_skip");
}

#endif