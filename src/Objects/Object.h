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

    //check if ray intersects given object; to be implemented in children
    virtual bool intersect(const vec3 &, const vec3 &, float &, uint32_t &, Vec2 &) const = 0;
    //get color properties
    virtual vec3 evalDiffuseColor(const Vec2 &) const;

    //properties, such as N (normal)
    virtual void getSurfaceProperties(const vec3 &, const vec3 &, const uint32_t &, const Vec2 &, vec3 &, Vec2 &) const = 0;

    MaterialType materialType;
    float ior;
    float Kd, Ks;
    vec3 diffuseColor;
    float specularExponent;
};


#endif //RT_OBJECT_H
