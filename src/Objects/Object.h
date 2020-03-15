//
// Created by andrez on 14.03.2020.
//

#ifndef RT_OBJECT_H
#define RT_OBJECT_H

#include "../Vectors/vectors.h"
#include "../misc/misc.h"

class Object {
public:
    Object();
    virtual ~Object() {}
    virtual bool intersect(const Vec3f &, const Vec3f &, float &, uint32_t &, Vec2f &) const = 0;
    virtual void getSurfaceProperties(const Vec3f &, const Vec3f &, const uint32_t &, const Vec2f &, Vec3f &, Vec2f &) const = 0;
    virtual Vec3f evalDiffuseColor(const Vec2f &) const;

    // material properties
    MaterialType materialType;
    float ior;
    float Kd, Ks;
    Vec3f diffuseColor;
    float specularExponent;
};


#endif //RT_OBJECT_H
