//
// Created by andrez on 14.03.2020.
//

#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#include "Object.h"

class Sphere : public Object{
public:
    Sphere(const Vec3 &c, const float &r, MaterialType type = OPAQUE, float ior = 1.3, Vec3 color = {1, 1, 1});
    bool intersect(const Vec3 &orig, const Vec3 &dir, float &tnear, uint32_t &index, Vec2 &uv) const;

    void getSurfaceProperties(const Vec3 &P, const Vec3 &I, const uint32_t &index, const Vec2 &uv, Vec3 &N, Vec2 &st) const;

    Vec3 center;
    float radius, radius2;
};




#endif //RT_SPHERE_H
