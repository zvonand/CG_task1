//
// Created by andrez on 14.03.2020.
//

#include "misc.h"
#include <cmath>
#include <utility>
#include <functional>

//TODO: maybe migrate to doubles

vec3 toUnitVector (const vec3 &v) {
    float nd = v.x * v.x + v.y * v.y + v.z * v.z;
    if (nd > 0) {
        float invLen = 1 / sqrtf(nd);
        return {v.x * invLen, v.y * invLen, v.z * invLen};
    }
    return v;
}

float scalarProduct(const vec3 &a, const vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 crossProduct(const vec3 &a, const vec3 &b) {
    return {a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x};
}

float clamp(const float &lo, const float &hi, const float &v) {
    return std::max(lo, std::min(hi, v));
}

float deg2rad(const float &deg) {
    return deg * M_PI / 180;
}

float rad2deg(const float &rad) {
    return rad * 180 /M_PI;
}

vec3 mix(const vec3 &pa, const vec3 &pb, const float &mixValue) {//for checkboard coloring
    return pa * (1 - mixValue) + pb * mixValue;
}

//solve quadratic equation with discriminant
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

vec3 reflect(const vec3 &I, const vec3 &N) {    //reflected dir
    return I - 2 * scalarProduct(I, N) * N;
}

vec3 refract(const vec3 &I, const vec3 &N, const float &ior) {  //refracted dir
    float cosi = clamp(-1, 1, scalarProduct(I, N));
    float etai = 1, etat = ior;
    vec3 n = N; //incoming nm

    if (cosi >= 0) {    //inverting factors
        std::swap(etai, etat);
        n= -N;
    } else {
        cosi = std::abs(cosi);
    }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
}

Options setOptions(std::string filename, uint32_t width, uint32_t height) {
    Options options;
    options.width = width*2;    //x2 for antialiasing
    options.height = height*2;
    options.viewField = 60;
    options.backColor = vec3(0.3, 0.1, 0.1);
    options.maxDepth = 5;  //recursion
    options.bias = 0.001;  //slight, small value
    options.filename = std::move(filename);
    return options;
}

bool rayTriangleIntersect(const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec3 &orig, const vec3 &dir,
                          float &tnear, float &u, float &v) {
    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;
    vec3 pvec = crossProduct(dir, edge2);
    float det = scalarProduct(edge1, pvec);
    if (det == 0 || det < 0) return false;

    vec3 tvec = orig - v0;
    u = scalarProduct(tvec, pvec);
    if (u < 0 || u > det) return false;

    vec3 qvec = crossProduct(tvec, edge1);
    v = scalarProduct(dir, qvec);
    if (v < 0 || u + v > det) return false;

    float invDet = 1 / det;

    tnear = scalarProduct(edge2, qvec) * invDet;
    u *= invDet;
    v *= invDet;

    return true;
}

void applyFresnel(const vec3 &I, const vec3 &N, const float &ior, float &kr) {
    float cosi = clamp(-1, 1, scalarProduct(I, N));
    float etai = 1, etat = ior;
    if (cosi > 0) {  std::swap(etai, etat); }
    // sini computation mit Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));

    kr = [&] {
        if (sint >= 1) {
            return (float)1;
        } else {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            return (Rs * Rs + Rp * Rp) / 2;
        }
    }();

    //Due to energy conservation law, the formula is as given:
    // kt = 1 - kr;
}

