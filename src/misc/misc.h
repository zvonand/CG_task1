//
// Created by andrez on 14.03.2020.
//

#ifndef RT_MISC_H
#define RT_MISC_H

#include <limits>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>

#include "../Vectors/vectors.h"

const float infty = std::numeric_limits<float>::max();
enum MaterialType { OPAQUE, MIRROR, GLASS};

struct Pixel {
    unsigned char b=0, g=0, r=0;
};

struct Options {
    uint32_t width;
    uint32_t height;
    float viewField;
    uint8_t maxDepth;
    vec3 backColor;
    float bias;
    std::string filename;
};

vec3 toUnitVector(const vec3 &v);

float scalarProduct(const vec3 &a, const vec3 &b);

vec3 crossProduct(const vec3 &a, const vec3 &b);

float clamp(const float &lo, const float &hi, const float &v);

float deg2rad(const float &deg);

vec3 mix(const vec3 &a, const vec3& b, const float &mixValue);

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

vec3 reflect(const vec3 &I, const vec3 &N);

vec3 refract(const vec3 &I, const vec3 &N, const float &ior);

Options setOptions(std::string filename, uint32_t width = 1024, uint32_t height = 780);

bool rayTriangleIntersect(
        const vec3 &v0, const vec3 &v1, const vec3 &v2,
        const vec3 &orig, const vec3 &dir,
        float &tnear, float &u, float &v);

void applyFresnel(const vec3 &I, const vec3 &N, const float &ior, float &kr);  //Fresnel equations




#endif
