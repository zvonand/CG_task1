//
// Created by andrez on 14.03.2020.
//

#include "MeshTriangle.h"

MeshTriangle::MeshTriangle(const Vec3 *verts, const uint32_t *vertsIndex, const uint32_t &numTris, const Vec2 *st) {
    uint32_t maxIndex = 0;
    for (uint32_t i = 0; i < numTris * 3; ++i)
        if (vertsIndex[i] > maxIndex) maxIndex = vertsIndex[i];
    maxIndex += 1;
    vertices = std::unique_ptr<Vec3[]>(new Vec3[maxIndex]);
    memcpy(vertices.get(), verts, sizeof(Vec3) * maxIndex);
    vertexIndex = std::unique_ptr<uint32_t[]>(new uint32_t[numTris * 3]);
    memcpy(vertexIndex.get(), vertsIndex, sizeof(uint32_t) * numTris * 3);
    numTriangles = numTris;

}

bool MeshTriangle::intersect(const Vec3 &orig, const Vec3 &dir, float &tnear, uint32_t &index, Vec2 &uv) const {
    bool intersect = false;
    #pragma omp parallel for
    for (uint32_t k = 0; k < numTriangles; ++k) {
        const Vec3 & v0 = vertices[vertexIndex[k * 3]];
        const Vec3 & v1 = vertices[vertexIndex[k * 3 + 1]];
        const Vec3 & v2 = vertices[vertexIndex[k * 3 + 2]];
        float t, u, v;
        if (rayTriangleIntersect(v0, v1, v2, orig, dir, t, u, v) && t < tnear) {
            tnear = t;
            uv.x = u;
            uv.y = v;
            index = k;
            intersect |= true;
        }
    }

    return intersect;
}

void
MeshTriangle::getSurfaceProperties(const Vec3 &P, const Vec3 &I, const uint32_t &index, const Vec2 &uv, Vec3 &N, Vec2 &st) const {
    const Vec3 &v0 = vertices[vertexIndex[index * 3]];
    const Vec3 &v1 = vertices[vertexIndex[index * 3 + 1]];
    const Vec3 &v2 = vertices[vertexIndex[index * 3 + 2]];
    Vec3 e0 = normalize(v1 - v0);
    Vec3 e1 = normalize(v2 - v1);
    N = normalize(crossProduct(e0, e1));
}


