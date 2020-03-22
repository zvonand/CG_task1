//
// Created by andrez on 14.03.2020.
//

#include "Object.h"
#include "../Vectors/vectors.h"
#include "../misc/misc.h"

Object::Object() : materialType(OPAQUE), ior(1.5), Kd(0.8), Ks(0.2), diffuseColor(vec3{0.2, 0.2, 0.2}), specularExponent(25) {}

vec3 Object::evalDiffuseColor(const Vec2 &) const {
    return diffuseColor;
}
