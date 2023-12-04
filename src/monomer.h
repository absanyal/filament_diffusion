#ifndef MONOMER_H
#define MONOMER_H

#include <vector>
#include "vectormaths.h"

using namespace std;

class monomer
{
public:
    vector<double> pos;
    vector<double> force;
    vector<double> head;
    vector<double> neighbors;
    double radius, mass;

    void add_neighbor(double);
    void reset_pos(), reset_force(), reset_head();
    void set_pos(double, double, double);
    void set_force(double, double, double);
    void set_head(double, double, double);

    monomer(double _radius = 1.0, double _mass = 1.0)
        : radius(_radius),
          mass(_mass)
    {
        pos.resize(3);
        force.resize(3);
        head.resize(3);
    }
};

void monomer::reset_pos()
{
    pos[0] = 0.0;
    pos[1] = 0.0;
    pos[2] = 0.0;
}

void monomer::set_pos(double x, double y, double z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void monomer::reset_force()
{
    force[0] = 0.0;
    force[1] = 0.0;
    force[2] = 0.0;
}

void monomer::set_force(double x, double y, double z)
{
    force[0] = x;
    force[1] = y;
    force[2] = z;
}

void monomer::reset_head()
{
    head[0] = 0.0;
    head[1] = 0.0;
    head[2] = 0.0;
}

void monomer::set_head(double x, double y, double z)
{
    head[0] = x;
    head[1] = y;
    head[2] = z;
}

void monomer::add_neighbor(double i)
{
    neighbors.push_back(i);
}

#endif