#ifndef VECTORMATHS_HPP
#define VECTORMATHS_HPP

#include <vector>
#include <cassert>
#include <cmath>

using namespace std;
typedef vector<double> vd;

string printvec(vd const &v)
{
    string result;
    result = "(";
    for (int i = 0; i < v.size(); i++)
    {
        if (i > 0)
        {
            result += ", ";
        }
        result += to_string(v[i]);
    }
    result += ") ";
    return result;
}

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
        prefactor =  (1.0 / (norm(v)));
    }
    else
    {
        prefactor = 1.0;
    }
    return prefactor * v;
}

#endif