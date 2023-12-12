#ifndef FTSZ_H
#define FTSZ_H

#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>
#include "vectormaths.h"
#include "monomer.h"

#define T_state 0
#define R_state 1

class ftsz : public monomer
{
public:
    bool GTP;
    int state;
    ftsz(double radius = 1.0, double mass = 1.0)
    {
        monomer::mass = mass;
        monomer::radius = radius;
    }
};

#endif