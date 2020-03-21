//
// Created by andrez on 19.03.2020.
//

#include "Pyramid.h"

uint32_t pVertIndex[12] = {0, 1, 2, 0, 1, 3, 0, 2, 3};
Vec2 p_st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

Pyramid::Pyramid(const Vec3 *verts) : MeshTriangle(verts, pVertIndex, 4, p_st) {
    materialType = OPAQUE;
    diffuseColor = {1, 0, 0};
}
