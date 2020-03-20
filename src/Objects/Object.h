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
    virtual bool intersect(const Vec3 &, const Vec3 &, float &, uint32_t &, Vec2 &) const = 0;
    virtual void getSurfaceProperties(const Vec3 &, const Vec3 &, const uint32_t &, const Vec2 &, Vec3 &, Vec2 &) const = 0;
    virtual Vec3 evalDiffuseColor(const Vec2 &) const;

    // material properties
    MaterialType materialType;
    float ior;
    float Kd, Ks;
    Vec3 diffuseColor;
    float specularExponent;
};


#endif //RT_OBJECT_H
