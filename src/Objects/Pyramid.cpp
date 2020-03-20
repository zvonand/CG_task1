//
// Created by andrez on 19.03.2020.
//

#include "Pyramid.h"

uint32_t vertIndex[12] = {0, 1, 2, 0, 1, 3, 0, 2, 3};

Pyramid::Pyramid(const Vec3 *verts, const uint32_t *vertsIndex, const uint32_t &numTris, const Vec2 *st
                 ) : MeshTriangle(verts, vertsIndex, 4, st) {

}
