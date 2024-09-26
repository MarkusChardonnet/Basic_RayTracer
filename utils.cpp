#include "utils.h"

using namespace std;

array<float, 4> Plane(Vec3f A, Vec3f B, Vec3f C) {
    Vec3f normal = normalize(cross(B - A, C - A));
    float d = -(normal[0] * A[0] + normal[1] * A[1] + normal[2] * A[2]);
    array<float, 4> p = {normal[0], normal[1], normal[2], d};
    return p;
};


bool SameSide(Vec3f p1, Vec3f p2, Vec3f a, Vec3f b) {
    Vec3f cp1 = cross(b-a, p1-a);
    Vec3f cp2 = cross(b-a, p2-a);
    if (dot(cp1, cp2) >= 0)
        return true;
    return false;
};

bool PointInTriangle(Vec3f p, Vec3f a, Vec3f b, Vec3f c) {
    if (SameSide(p,a,b,c) && SameSide(p,b,a,c) && SameSide(p,c,a,b))
        return true;
    return false;
};
