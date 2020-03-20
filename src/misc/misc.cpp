//
// Created by andrez on 14.03.2020.
//

#include "misc.h"
#include <cmath>

Vec3 normalize(const Vec3 &v) {
    float mag2 = v.x * v.x + v.y * v.y + v.z * v.z;
    if (mag2 > 0) {
        float invMag = 1 / sqrtf(mag2);
        return {v.x * invMag, v.y * invMag, v.z * invMag};
    }
    return v;
}

float dotProduct(const Vec3 &a, const Vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 crossProduct(const Vec3 &a, const Vec3 &b) {
    return {a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x};
}

float clamp(const float &lo, const float &hi, const float &v) {
    return std::max(lo, std::min(hi, v));
}

float deg2rad(const float &deg) {
    return deg * M_PI / 180;
}

Vec3 mix(const Vec3 &a, const Vec3 &b, const float &mixValue) {
    return a * (1 - mixValue) + b * mixValue;
}

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) {
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = - 0.5 * b / a;
    else {
        float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    return true;
}

Vec3 reflect(const Vec3 &I, const Vec3 &N) {
    return I - 2 * dotProduct(I, N) * N;
}

Vec3 refract(const Vec3 &I, const Vec3 &N, const float &ior) {
    float cosi = clamp(-1, 1, dotProduct(I, N));
    float etai = 1, etat = ior;
    Vec3 n = N;
    if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -N; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
}

Options setOptions(std::string filename, uint32_t width, uint32_t height) {
    Options options;
    options.width = width*2;
    options.height = height*2;
    options.fov = 90;
    options.backgroundColor = Vec3(0.1, 0.1, 0.1);
    options.maxDepth = 4;  //maximum recursion depth
    options.bias = 0.0001;
    options.filename = filename;
    return options;
}

bool rayTriangleIntersect(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &orig, const Vec3 &dir,
                          float &tnear, float &u, float &v) {
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    Vec3 pvec = crossProduct(dir, edge2);
    float det = dotProduct(edge1, pvec);
    if (det == 0 || det < 0) return false;

    Vec3 tvec = orig - v0;
    u = dotProduct(tvec, pvec);
    if (u < 0 || u > det) return false;

    Vec3 qvec = crossProduct(tvec, edge1);
    v = dotProduct(dir, qvec);
    if (v < 0 || u + v > det) return false;

    float invDet = 1 / det;

    tnear = dotProduct(edge2, qvec) * invDet;
    u *= invDet;
    v *= invDet;

    return true;
}

void fresnel(const Vec3 &I, const Vec3 &N, const float &ior, float &kr) {
    float cosi = clamp(-1, 1, dotProduct(I, N));
    float etai = 1, etat = ior;
    if (cosi > 0) {  std::swap(etai, etat); }
    // sini computation with Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    //Due to energy conservation law, the formula is as given:
    // kt = 1 - kr;
}
