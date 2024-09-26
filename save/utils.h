#include <cmath>
#include <array>
#include "Vec3.cpp"

#pragma once

bool PointInTriangle(Vec3f p, Vec3f a, Vec3f b, Vec3f c);
bool SameSide(Vec3f p1, Vec3f p2, Vec3f a, Vec3f b);
std::array<float, 4> Plane(Vec3f A, Vec3f B, Vec3f C);
