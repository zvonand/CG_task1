//
// Created by andrez on 16.03.2020.
//

#include "Checkboard.h"

uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};
Vec2 st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};

Checkboard::Checkboard(const Vec3 *verts)
        : MeshTriangle(verts, vertIndex, 2, st) {
    uint32_t maxIndex = 0;
    for (uint32_t i = 0; i < 6; ++i)
        if (vertIndex[i] > maxIndex) maxIndex = vertIndex[i];
    maxIndex += 1;
    materialType = OPAQUE;
}

Vec3 Checkboard::evalDiffuseColor(const Vec2 &st) const {
    float scale = 50;
    float pattern = (fmodf(st.x * scale, 1) > 0.5) ^ (fmodf(st.y * scale, 1) > 0.5);
    return mix(Vec3(0.1, 0.1, 0.1), Vec3(0.92, 0.9, 0.9), pattern);
}
