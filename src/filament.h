#ifndef FILAMENT_H
#define FILAMENT_H

#include <vector>
#include "vectormaths.h"
#include "ftsz.h"

using namespace std;

class filament
{
private:
    vector<double> start_coord, end_coord;

public:
    vector<ftsz> monomers;
    vector<double> com_coord;
    vector<double> heading;
    vector<double> force;

    int size();
    double totalmass();
    void set_heading(double, double);

    filament(int length, double radius, double mass)
    {
        monomers.resize(length);
        com_coord.resize(3);
        heading.resize(2);
        force.resize(3);
        start_coord.resize(3);
        end_coord.resize(3);

        for (int i = 0; i < monomers.size(); i++)
        {
            monomers[i].mass = mass;
            monomers[i].radius = radius;
        }
    }
};

int filament::size()
{
    return monomers.size();
}

void filament::set_heading(double theta, double phi)
{
    heading[0] = theta;
    heading[1] = phi;
}

double filament::totalmass()
{
    double fmass = 0.0;
    for (int i = 0; i < size(); i++)
    {
        fmass += monomers[i].mass;
    }
    return fmass;
}

#endif