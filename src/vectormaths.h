#ifndef VECTORMATHS_HPP
#define VECTORMATHS_HPP

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;
typedef vector<double> vd;

// string printvec(vd const &v)
// {
//     string result;
//     result = "(";
//     for (int i = 0; i < v.size(); i++)
//     {
//         if (i > 0)
//         {
//             result += ", ";
//         }
//         result += to_string(v[i]);
//     }
//     result += ") ";
//     return result;
// }

vd operator+(vd const &v1, vd const &v2)
{
    assert(v1.size() == v2.size());
    vd result(v1.size());
    for (int i = 0; i < result.size(); i++)
    {
        result[i] = v1[i] + v2[i];
    }
    return result;
}

vd operator-(vd const &v1, vd const &v2)
{
    assert(v1.size() == v2.size());
    vd result(v1.size());
    for (int i = 0; i < result.size(); i++)
    {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

vd operator-(vd const &v)
{
    vd result(v.size());
    for (int i = 0; i < v.size(); i++)
    {
        result[i] = -v[i];
    }
    return result;
}

vd operator*(vd const &v, double const &a)
{
    vd result(v.size());
    for (int i = 0; i < result.size(); i++)
    {
        result[i] = a * v[i];
    }
    return result;
}

vd operator*(double const &a, vd const &v)
{
    vd result(v.size());
    for (int i = 0; i < result.size(); i++)
    {
        result[i] = a * v[i];
    }
    return result;
}

vd operator/(vd const &v, double const &a)
{
    vd result(v.size());
    for (int i = 0; i < result.size(); i++)
    {
        result[i] = (1.0 / a) * v[i];
    }
    return result;
}

ostream &operator<<(ostream &os, vd const &a)
{
    os << "(" << a[0] << ", " << a[1] << ", " << a[2]
       << ")";
    return os;
}

double dot(vd const &v1, vd const &v2)
{
    assert(v1.size() == v2.size());
    double result;
    result = 0.0;
    for (int i = 0; i < v1.size(); i++)
    {
        result += v1[i] * v2[i];
    }
    return result;
}

double norm(vd const &v)
{
    return sqrt(dot(v, v));
}

vd normalize(vd const &v)
{
    double prefactor;
    if (norm(v) != 0.0)
    {
        prefactor = (1.0 / (norm(v)));
    }
    else
    {
        prefactor = 1.0;
    }
    return prefactor * v;
}

vd cross3D(vd a, vd b)
{
    assert(a.size() == b.size());
    assert(a.size() == 3);
    double a1, a2, a3, b1, b2, b3;
    vd result;
    a1 = a[0];
    a2 = a[1];
    a3 = a[2];
    b1 = b[0];
    b2 = b[1];
    b3 = b[2];
    result.resize(3);
    result[0] = a2 * b3 - a3 * b2;
    result[1] = a3 * b1 - a1 * b3;
    result[2] = a1 * b2 - a2 * b1;
    return result;
}

vd convertbasis(vd start_vec, vd x, vd y, vd z, vd X, vd Y, vd Z)
{
    vd exp_x, exp_y, exp_z, result;
    exp_x = (dot(X, x)) * X + (dot(Y, x)) * Y + (dot(Z, x)) * Z;
    exp_y = (dot(X, y)) * X + (dot(Y, y)) * Y + (dot(Z, y)) * Z;
    exp_z = (dot(X, z)) * X + (dot(Y, z)) * Y + (dot(Z, z)) * Z;

    result = start_vec[0] * exp_x +
             start_vec[1] * exp_y +
             start_vec[2] * exp_z;

    return result;
}

#endif