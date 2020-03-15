//
// Created by andrez on 14.03.2020.
//

#ifndef RT_MISC_H
#define RT_MISC_H

#include <limits>
#include <string>
#include <cmath>
#include "../Vectors/vectors.h"

const uint32_t RED = 0x000000FF;
const uint32_t GREEN = 0x0000FF00;
const uint32_t BLUE = 0x00FF0000;

const float kInfinity = std::numeric_limits<float>::max();
enum MaterialType { OPAQUE, MIRROR, REFLECTION_AND_REFRACTION, REFRACTION };

struct Pixel { unsigned char r, g, b; };

struct Options {
    uint32_t width;
    uint32_t height;
    float fov;
    float imageAspectRatio;
    uint8_t maxDepth;
    Vec3f backgroundColor;
    float bias;
    std::string filename;
};

Vec3f normalize(const Vec3f &v);

float dotProduct(const Vec3f &a, const Vec3f &b);

Vec3f crossProduct(const Vec3f &a, const Vec3f &b);

float clamp(const float &lo, const float &hi, const float &v);

float deg2rad(const float &deg);

Vec3f mix(const Vec3f &a, const Vec3f& b, const float &mixValue);

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

Vec3f reflect(const Vec3f &I, const Vec3f &N);

Vec3f refract(const Vec3f &I, const Vec3f &N, const float &ior);

Options setOptions(std::string filename, uint32_t width = 1000, uint32_t height = 600);

bool rayTriangleIntersect(
        const Vec3f &v0, const Vec3f &v1, const Vec3f &v2,
        const Vec3f &orig, const Vec3f &dir,
        float &tnear, float &u, float &v);

void fresnel(const Vec3f &I, const Vec3f &N, const float &ior, float &kr);  //Fresnel equations


#endif
