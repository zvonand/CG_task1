//
// Created by andrez on 19.03.2020.
//

#ifndef RT_PYRAMID_H
#define RT_PYRAMID_H

#include "../Vectors/vectors.h"
#include "../Objects/Object.h"
#include "MeshTriangle.h"


class Pyramid : public MeshTriangle {
    Pyramid(const Vec3 *verts, const uint32_t *vertsIndex, const uint32_t &numTris, const Vec2 *st);
};


#endif //RT_PYRAMID_H
