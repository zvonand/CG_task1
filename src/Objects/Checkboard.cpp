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
    stCoordinates = std::unique_ptr<Vec2[]>(new Vec2[maxIndex]);
    memcpy(stCoordinates.get(), st, sizeof(Vec2) * maxIndex);
    materialType = OPAQUE;
}

Vec3 Checkboard::evalDiffuseColor(const Vec2 &st) const {
    float scale = 50;
    float pattern = (fmodf(st.x * scale, 1) > 0.5) ^ (fmodf(st.y * scale, 1) > 0.5);
    return mix(Vec3(0.1, 0.1, 0.1), Vec3(0.92, 0.9, 0.9), pattern);
}

void Checkboard::getSurfaceProperties(const Vec3 &P, const Vec3 &I, const uint32_t &index, const Vec2 &uv, Vec3 &N,
                                      Vec2 &st) const {
    const Vec3 &v0 = vertices[vertexIndex[index * 3]];
    const Vec3 &v1 = vertices[vertexIndex[index * 3 + 1]];
    const Vec3 &v2 = vertices[vertexIndex[index * 3 + 2]];
    Vec3 e0 = normalize(v1 - v0);
    Vec3 e1 = normalize(v2 - v1);
    N = normalize(crossProduct(e0, e1));
    const Vec2 &st0 = stCoordinates[vertexIndex[index * 3]];
    const Vec2 &st1 = stCoordinates[vertexIndex[index * 3 + 1]];
    const Vec2 &st2 = stCoordinates[vertexIndex[index * 3 + 2]];
    st = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;
}

