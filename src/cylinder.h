#pragma once

#include "common_objects.h"
#include "vectormaths.h"
#include "parameters.h"

bool check_inside_cylinder(double R, vd rA, vd rB, vd rP)
{
     vd e, m, rQ;
     double d;
     double wA, wB;
     double rArA, rArB, rBrB, rQrA, rQrB;

     bool inside = true;

     e = rA - rB;
     m = cross3D(rA, rB);
     d = norm(cross3D(e, rP - rA)) / norm(e);

     rQ = rP + ((cross3D(e, m + cross3D(e, rP))) / (pow(norm(e), 2)));

     rArA = dot(rA, rA);
     rArB = dot(rA, rB);
     rBrB = dot(rB, rB);
     rQrA = dot(rQ, rA);
     rQrB = dot(rQ, rB);

     wA = -(rArB - rBrB - rQrA - rBrB * rQrA + rQrB + rArB * rQrB) / (rArA - 2.0 * rArB - pow(rArB, 2) + rBrB + rArA * rBrB);

     wB = -(rArA - rArB - rQrA - rArB * rQrA + rQrB + rArA * rQrB) /
          (-rArA + 2.0 * rArB + pow(rArB, 2) - rBrB - rArA * rBrB);

     if (d > R || wA < 0.0 || wA > 1.0 || wB < 0.0 || wB > 1.0)
     {
          inside = false;
     }

     return inside;
}