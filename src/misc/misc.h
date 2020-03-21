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
enum MaterialType { OPAQUE, MIRROR, GLASS};

struct Pixel { unsigned char b, g, r; };

struct Options {
    uint32_t width;
    uint32_t height;
    float fov;
    uint8_t maxDepth;
    Vec3 backgroundColor;
    float bias;
    std::string filename;
};

Vec3 normalize(const Vec3 &v);

float dotProduct(const Vec3 &a, const Vec3 &b);

Vec3 crossProduct(const Vec3 &a, const Vec3 &b);

float clamp(const float &lo, const float &hi, const float &v);

float deg2rad(const float &deg);

Vec3 mix(const Vec3 &a, const Vec3& b, const float &mixValue);

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

Vec3 reflect(const Vec3 &I, const Vec3 &N);

Vec3 refract(const Vec3 &I, const Vec3 &N, const float &ior);

Options setOptions(std::string filename, uint32_t width = 1280, uint32_t height = 720);

bool rayTriangleIntersect(
        const Vec3 &v0, const Vec3 &v1, const Vec3 &v2,
        const Vec3 &orig, const Vec3 &dir,
        float &tnear, float &u, float &v);

void applyFresnel(const Vec3 &I, const Vec3 &N, const float &ior, float &kr);  //Fresnel equations


#endif
