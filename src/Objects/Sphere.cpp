//
// Created by andrez on 14.03.2020.
//

#include "Sphere.h"

Sphere::Sphere(const Vec3 &c, const float &r, MaterialType type, float ior_t, Vec3 color) : center(c), radius(r), radius2(r * r) {
    materialType = type;
    ior = ior_t;
    diffuseColor = color;
}

bool Sphere::intersect(const Vec3 &orig, const Vec3 &dir, float &tnear, uint32_t &index, Vec2 &uv) const {
    Vec3 L = orig - center;
    float a = dotProduct(dir, dir);
    float b = 2 * dotProduct(dir, L);
    float c = dotProduct(L, L) - radius2;
    float t0, t1;
    if (!solveQuadratic(a, b, c, t0, t1)) return false;
    if (t0 < 0) t0 = t1;
    if (t0 < 0) return false;
    tnear = t0;

    return true;
}

void Sphere::getSurfaceProperties(const Vec3 &P, const Vec3 &I, const uint32_t &index, const Vec2 &uv, Vec3 &N, Vec2 &st) const {
    N = normalize(P - center);
}
