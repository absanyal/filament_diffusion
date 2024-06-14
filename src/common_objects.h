#pragma once

#include <random>

#include "parameters.h"
#include "vectormaths.h"

#define PI acos(-1.0)

#define sigma_scale 1.0
#define epsilon_scale 1.0
#define nano 1E-9
#define micro 1E-6
#define milli 1E-3

#define kB0 1.38E-23
#define T0 310.0
#define eta0 1E-3
#define sigma0 1E-9

parameters prm;

std::mt19937 gen;
bool rng_seeded = false;
std::normal_distribution<double> dis_normal(0.0, 1.0);

vd X = {1.0, 0.0, 0.0};
vd Y = {0.0, 1.0, 0.0};
vd Z = {0.0, 0.0, 1.0};

int wasted_steps = 0;