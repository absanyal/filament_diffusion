#ifndef FILAMENT_H
#define FILAMENT_H

#include <vector>
#include "vectormaths.h"
#include "ftsz.h"
#include "quaternion.h"
#include <string>

#define PI acos(-1.0)

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
    void rotate_filament(double, string, bool);
    vd get_CoM();
    vd face();
    double radius_of_gyration();

    filament(int length, double radius, double mass, vd start_vector, vd heading_vector)
    {
        monomers.resize(length);
        heading.resize(3);
        _face.resize(3);
        force.resize(3);
        start_coord.resize(3);

        assert(start_coord.size() == start_vector.size());
        start_coord = start_vector;

        assert(heading.size() == heading_vector.size());
        heading = normalize(heading_vector);
        assert(norm(heading) > 0.0);

        if (heading[0] != 0.0)
        {
            _face = {-(heading[1] + heading[2]) / heading[0], 1, 1};
        }
        if (heading[0] == 0 && heading[1] != 0)
        {
            _face = {1, -heading[2] / heading[1], 1};
        }
        if (heading[0] == 0 && heading[1] == 0)
        {
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
    countourlen = 2.0 * prm.radius;
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

vd filament::face()
{
    return _face;
}

void filament::rotate_filament(double angle, string local_axis, bool degree = false)
{
    if (degree == true)
    {
        angle = angle * (PI / 180);
    }

    vd x, y, z, X, Y, Z;
    vd axis;
    vd CoM_fixed;

    X = {1.0, 0.0, 0.0};
    Y = {0.0, 1.0, 0.0};
    Z = {0.0, 0.0, 1.0};

    z = heading;
    x = _face;
    y = cross3D(z, x) / norm(cross3D(z, x));

    CoM_fixed = get_CoM();

    if (local_axis == "x")
    {
        axis = x;
    }
    else if (local_axis == "y")
    {
        axis = y;
    }
    else if (local_axis == "z")
    {
        axis = z;
    }
    else
    {
        axis = {0.0, 0.0, 0.0};
    }

    quaternion q, p, p_prime;
    vd current_pos_local, current_pos_global;

    // Set the rotation quaternion
    q.scalar = cos(angle / 2.0);
    q.vector = sin(angle / 2.0) * axis;

    // Rotate the heading vector
    p.set_components(0.0, heading);
    p_prime = q * p * q.inverse();
    heading = p_prime.vector;

    // Rotate the face vector
    p.set_components(0.0, _face);
    p_prime = q * p * q.inverse();
    _face = p_prime.vector;

    // Rotate every monomer in the filament
    for (int i = 0; i < length(); i++)
    {
        p.set_components(0.0, monomers[i].pos - CoM_fixed);
        p_prime = q * p * q.inverse();
        monomers[i].pos = p_prime.vector + CoM_fixed;
    }

    start_coord = monomers[0].pos;
}

double filament::radius_of_gyration()
{
    double rg;

    vd fixed_CoM;
    fixed_CoM = get_CoM();

    double total_mass;
    total_mass = 0.0;

    double total_moment;
    total_moment = 0.0;

    for (int i = 0; i < length(); i++)
    {
        total_mass += monomers[i].mass;

        total_moment += monomers[i].mass * pow(norm(monomers[i].pos - fixed_CoM), 2);
    }

    rg = total_moment / total_mass;

    return rg;
}

#endif