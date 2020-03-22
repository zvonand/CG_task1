//
// Created by andrez on 14.03.2020.
//

#include "Sphere.h"

Sphere::Sphere(const vec3 &c, const float &r, MaterialType type, vec3 color, float ior_t) : centre(c), radius(r), sqrdRadius(r * r) {
    materialType = type;
    ior = ior_t;
    diffuseColor = color;
}

bool Sphere::intersect(const vec3 &orig, const vec3 &dir, float &tnear, uint32_t &index, Vec2 &uv) const {
    float t0, t1;

    vec3 L = orig - centre;
    float a = scalarProduct(dir, dir);
    float b = 2 * scalarProduct(dir, L);
    float c = scalarProduct(L, L) - sqrdRadius;

    if (!solveQuadratic(a, b, c, t0, t1)) return false;
    if (t0 < 0) t0 = t1;
    if (t0 < 0) return false;
    tnear = t0;
    return true;
}

void Sphere::getSurfaceProperties(const vec3 &P, const vec3 &I, const uint32_t &index, const Vec2 &uv, vec3 &N, Vec2 &st) const {
    N = toUnitVector(P - centre);
}
