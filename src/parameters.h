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

    double eta;

    double k_react;

    bool wall_collisions;
    bool manual_diffusion_constants;
};

/**
 * This function reads a file line by line and searches for a specific string match followed by an '=' sign.
 * If the match is found, it extracts the corresponding value and returns it as a double.
 * If the match is not found, it prints an error message indicating that the argument is missing in the input file.
 *
 * @param file The path of the file to be read.
 * @param match The string to be matched in the file.
 * @return The value corresponding to the matched string.
 */
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

/**
 * @brief This function reads a file and searches for a specific string match.
 *
 * @param file The path of the file to be read.
 * @param match The string to be searched for in the file.
 * @return The string value found after the match.
 */
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

/**
 * @brief Loads the parameters from the specified input file.
 *
 * @param inputfile The path to the input file.
 */
void parameters::load(string inputfile)
{
    seed = matchstring(inputfile, "seed");

    length = matchstring(inputfile, "length(subunits)");
    radius = matchstring(inputfile, "radius(nm)");
    mass = matchstring(inputfile, "mass(FtsZ mass)");

    iterations = matchstring(inputfile, "iterations");
    dt = matchstring(inputfile, "dt");

    // kB = matchstring(inputfile, "kB");
    T = matchstring(inputfile, "T(kelvin)");

    k_react = matchstring(inputfile, "k_react");

    wall_collisions = matchstring(inputfile, "wall_collisions");

    manual_diffusion_constants = matchstring(inputfile, "manual_diffusion_constants");

    D_par = matchstring(inputfile, "D_par(micro m^2/s)");
    D_perp = matchstring(inputfile, "D_perp(micro m^2/s)");
    D_rot = matchstring(inputfile, "D_rot(micro rad^2/s)");

    eta = matchstring(inputfile, "eta(mPa s)");

    xlo = matchstring(inputfile, "xlo(nm)");
    xhi = matchstring(inputfile, "xhi(nm)");
    cell_radius = matchstring(inputfile, "cell_radius(nm)");

    steps_to_skip = matchstring(inputfile, "steps_to_skip");
}

#endif