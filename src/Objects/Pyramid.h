//
// Created by andrez on 19.03.2020.
//

#ifndef RT_PYRAMID_H
#define RT_PYRAMID_H

#include "../Vectors/vectors.h"
#include "../Objects/Object.h"
#include "MeshTriangle.h"


class Pyramid : public MeshTriangle {
public:
    Pyramid(const Vec3 *verts);
};


#endif //RT_PYRAMID_H
