#ifndef FILAMENT_H
#define FILAMENT_H

#include <vector>
#include "vectormaths.h"
#include "ftsz.h"

using namespace std;

class filament
{
private:
    vector<double> start_coord, _face;

public:
    vector<ftsz> monomers;
    vector<double> heading;
    vector<double> force;

    int length();
    double totalmass();
    double contour_length();
    void displace_filament(vd);
    void add_at_beginning(double, double);
    void remove_at_end();
    vd get_CoM();
    vd face();

    filament(int length, double radius, double mass, vd start_vector, vd heading_vector)
    {
        monomers.resize(length);
        heading.resize(3);
        _face.resize(3);
        force.resize(3);
        start_coord.resize(3);

        assert(start_coord.size() == start_vector.size());
        start_coord = start_vector;

        // assert(dot(heading_vector, face_vector) == 0.0);

        assert(heading.size() == heading_vector.size());
        heading = normalize(heading_vector);
        assert(norm(heading) > 0.0);

        if (heading[0] != 0.0){
            _face = {-(heading[1] + heading[2])/heading[0], 1, 1 };
        }
        if ( heading[0] == 0 && heading[1] != 0 ){
            _face = {1, - heading[2] / heading[1], 1 };
        }
        if (heading[0] == 0 && heading[1] == 0){
            _face = {1.0, 0.0, 0.0};
        }

        _face = normalize(_face);

        for (int i = 0; i < monomers.size(); i++)
        {
            monomers[i].mass = mass;
            monomers[i].radius = radius;

            double previous_radius;
            if (i == 0)
            {
                previous_radius = 0.0;
            }
            else
            {
                previous_radius = monomers[i - 1].radius;
            }

            monomers[i].pos = start_coord + (-1.0 * i) * (previous_radius + monomers[i].radius) * heading;
        }
    }
};

int filament::length() // number of monomers in the filament
{
    return monomers.size();
}

double filament::contour_length()
{
    double countourlen;
    countourlen = 0.0;
    if (length() > 1)
    {
        for (int i = 1; i < length(); i++)
        {
            countourlen += norm(monomers[i].pos - monomers[i - 1].pos);
        }
    }
    return countourlen;
}

double filament::totalmass()
{
    double fmass = 0.0;
    for (int i = 0; i < length(); i++)
    {
        fmass += monomers[i].mass;
    }
    return fmass;
}

void filament::displace_filament(vd displacement_vector)
{
    for (int i = 0; i < length(); i++)
    {
        monomers[i].pos = monomers[i].pos + displacement_vector;
    }
    start_coord = start_coord + displacement_vector;
}

void filament::add_at_beginning(double radius, double mass)
{
    ftsz new_monomer(radius, mass);
    new_monomer.pos = start_coord + (monomers[1].radius + new_monomer.radius) * heading;
    monomers.insert(monomers.begin(), new_monomer);
    start_coord = new_monomer.pos;
    // cout << "Adding at: " << printvec(new_monomer.pos) << endl;
}

void filament::remove_at_end()
{
    monomers.pop_back();
}

vd filament::get_CoM()
{
    vd CoM;
    CoM.resize(3);

    for (int i = 0; i < length(); i++)
    {
        CoM = CoM + (monomers[i].mass * monomers[i].pos);
    }

    CoM = (1.0 / totalmass()) * CoM;
    return CoM;
}

vd filament::face(){
    return _face;
}

#endif