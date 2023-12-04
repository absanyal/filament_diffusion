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

    // float r;

    int iterations;
    float dt;

    float LJ_e, LJ_sigma, LJ_rc;
    float bond_k, bond_r0;

    float D;

    float kB, T;

    int steps_to_skip;

    // float xlo, xhi, ylo, yhi, zlo, zhi;

    int bond_break_message, stop_on_breakage;

    // float eta;

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

    // xlo = matchstring(inputfile, "xlo");
    // xhi = matchstring(inputfile, "xhi");

    // ylo = matchstring(inputfile, "ylo");
    // yhi = matchstring(inputfile, "yhi");

    // zlo = matchstring(inputfile, "zlo");
    // zhi = matchstring(inputfile, "zhi");
    

    // r = matchstring(inputfile, "r");

    iterations = matchstring(inputfile, "iterations");
    dt = matchstring(inputfile, "dt");

    D = matchstring(inputfile, "D");

    kB = matchstring(inputfile, "kB");
    T = matchstring(inputfile, "T");

    // eta = matchstring(inputfile, "eta");

    LJ_e = matchstring(inputfile, "LJ_e");
    LJ_sigma = matchstring(inputfile, "LJ_sigma");
    LJ_rc = matchstring(inputfile, "LJ_rc");

    bond_k = matchstring(inputfile, "bond_k");
    bond_r0 = matchstring(inputfile, "bond_r0");

    steps_to_skip = matchstring(inputfile, "steps_to_skip");

    bond_break_message = matchstring(inputfile, "bond_break_message");
    stop_on_breakage = matchstring(inputfile, "stop_on_breakage");
}

#endif