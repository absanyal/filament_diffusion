#pragma once

#include "vectormaths.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

class quaternion
{
public:
    double scalar;
    vd vector;

    void set_components(double, vd);
    quaternion conj(), inverse();
    double norm();

    quaternion(double scalar_part = 0.0, vd vector_part = {0.0, 0.0, 0.0})
    {
        scalar = scalar_part;
        vector = vector_part;
        assert(vector.size() == 3);
    }
};

quaternion operator+(quaternion const &a, quaternion const &b)
{
    quaternion result;
    result.scalar = a.scalar + b.scalar;
    result.vector = a.vector + b.vector;
    return result;
}

quaternion operator-(quaternion const &a, quaternion const &b)
{
    quaternion result;
    result.scalar = a.scalar - b.scalar;
    result.vector = a.vector - b.vector;
    return result;
}

quaternion operator*(quaternion const &a, quaternion const &b)
{
    quaternion result;
    result.scalar = a.scalar * b.scalar - dot(a.vector, b.vector);
    result.vector = a.scalar * b.vector +
                    b.scalar * a.vector +
                    cross3D(a.vector, b.vector);
    return result;
}

quaternion operator*(quaternion const &a, double const &s)
{
    quaternion result;
    result.scalar = a.scalar * s;
    result.vector = a.vector * s;
    return result;
}

quaternion operator*(double const &s, quaternion const &a)
{
    quaternion result;
    result.scalar = a.scalar * s;
    result.vector = a.vector * s;
    return result;
}

quaternion operator-(quaternion const &a)
{
    quaternion result;
    result.scalar = -a.scalar;
    result.vector = -a.vector;
    return result;
}

quaternion operator/(quaternion const &a, double const &s)
{
    quaternion result;
    result.scalar = a.scalar / s;
    result.vector = a.vector / s;
    return result;
}

ostream &operator<<(ostream &os, quaternion const &a)
{
    os << "[" << a.scalar << ", " << a.vector << "]";
    return os;
}

void quaternion::set_components(double scalar_part, vd vector_part)
{
    scalar = scalar_part;
    vector = vector_part;
}

quaternion quaternion::conj()
{
    quaternion result;
    result.scalar = scalar;
    result.vector = -vector;
    return result;
}

double quaternion::norm()
{
    return sqrt(scalar * scalar + dot(vector, vector));
}

quaternion quaternion::inverse()
{
    // assert(norm() != 0.0);
    return conj() / (norm() * norm());
}

double dot(const quaternion &a, const quaternion &b)
{
    return a.scalar * b.scalar + dot(a.vector, b.vector);
}