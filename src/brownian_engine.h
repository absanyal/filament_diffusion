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
    double displacement(vd);

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

double brownian_engine::displacement(vd _Fcons)
{
    double dr;
    double c1, c2;

    c1 = _D / (_kB * _T);

    return dr;
}

#endif