//
// Created by andrez on 14.03.2020.
//

#ifndef RT_SPHERE_H
#define RT_SPHERE_H

#define GLASS_IOR 1.45
#include "Object.h"

class Sphere : public Object{
public:
    Sphere(const vec3 &c, const float &r, MaterialType type = OPAQUE, vec3 color = {1, 1, 1}, float ior = GLASS_IOR);
    bool intersect(const vec3 &orig, const vec3 &dir, float &tnear, uint32_t &index, Vec2 &uv) const;
    void getSurfaceProperties(const vec3 &P, const vec3 &I, const uint32_t &index, const Vec2 &uv, vec3 &N, Vec2 &st) const;

    vec3 centre;
    float radius, sqrdRadius;
};




#endif //RT_SPHERE_H
