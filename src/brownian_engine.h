#ifndef BROWNIAN_ENGINE_H
#define BROWNIAN_ENGINE_H

#include <cmath>
#include "vectormaths.h"
#include <random>

class brownian_engine
{
private:
    double _dt, _D, _kB, _T, _seed;

public:
    double coordinate_displacement(double);

    brownian_engine(double dt, double D, double kB, double T, int seed)
    {
        _dt = dt;
        _D = D;
        _kB = kB;
        _T = T;
        _seed = seed;

        std::mt19937 gen(_seed);
        std::uniform_real_distribution<> unif(0, 1);
    }
};

double brownian_engine::coordinate_displacement(double _Fcons)
{
    double displacement;
    double c1, c2;

    return displacement;
}

#endif