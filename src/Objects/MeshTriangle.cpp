//
// Created by andrez on 14.03.2020.
//

#include "MeshTriangle.h"

MeshTriangle::MeshTriangle(const vec3 *verts, const uint32_t *vertsIndex, const uint32_t &numTris, const Vec2 *st) {
    uint32_t maxIndex = 0;
    for (uint32_t i = 0; i < numTris * 3; ++i)
        if (vertsIndex[i] > maxIndex) maxIndex = vertsIndex[i];
    maxIndex += 1;
    vertices = std::unique_ptr<vec3[]>(new vec3[maxIndex]);
    memcpy(vertices.get(), verts, sizeof(vec3) * maxIndex);
    vertexIndex = std::unique_ptr<uint32_t[]>(new uint32_t[numTris * 3]);
    memcpy(vertexIndex.get(), vertsIndex, sizeof(uint32_t) * numTris * 3);
    numTriangles = numTris;
    stCoordinates = std::unique_ptr<Vec2[]>(new Vec2[maxIndex]);
    memcpy(stCoordinates.get(), st, sizeof(Vec2) * maxIndex);

}

bool MeshTriangle::intersect(const vec3 &orig, const vec3 &dir, float &tnear, uint32_t &index, Vec2 &uv) const {
    bool intersect = false;
    #pragma omp parallel for
    for (uint32_t k = 0; k < numTriangles; ++k) {
        const vec3 & v0 = vertices[vertexIndex[k * 3]];
        const vec3 & v1 = vertices[vertexIndex[k * 3 + 1]];
        const vec3 & v2 = vertices[vertexIndex[k * 3 + 2]];
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
MeshTriangle::getSurfaceProperties(const vec3 &P, const vec3 &I, const uint32_t &index, const Vec2 &uv, vec3 &N, Vec2 &st) const {
    const vec3 &v0 = vertices[vertexIndex[index * 3]];
    const vec3 &v1 = vertices[vertexIndex[index * 3 + 1]];
    const vec3 &v2 = vertices[vertexIndex[index * 3 + 2]];
    vec3 e0 = toUnitVector(v1 - v0);
    vec3 e1 = toUnitVector(v2 - v1);
    N = toUnitVector(crossProduct(e0, e1));
    const Vec2 &st0 = stCoordinates[vertexIndex[index * 3]];
    const Vec2 &st1 = stCoordinates[vertexIndex[index * 3 + 1]];
    const Vec2 &st2 = stCoordinates[vertexIndex[index * 3 + 2]];
    st = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;
}


