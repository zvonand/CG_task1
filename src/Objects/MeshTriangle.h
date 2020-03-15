//
// Created by andrez on 14.03.2020.
//

#ifndef RT_MESHTRIANGLE_H
#define RT_MESHTRIANGLE_H

#include "Object.h"
#include <memory>
#include <cstring>
#include <cmath>

class MeshTriangle : public Object
{
public:
    MeshTriangle(
            const Vec3f *verts,
            const uint32_t *vertsIndex,
            const uint32_t &numTris,
            const Vec2f *st);

    bool intersect(const Vec3f &orig, const Vec3f &dir, float &tnear, uint32_t &index, Vec2f &uv) const;

    void getSurfaceProperties(const Vec3f &P, const Vec3f &I, const uint32_t &index, const Vec2f &uv, Vec3f &N, Vec2f &st) const;

    Vec3f evalDiffuseColor(const Vec2f &st) const;

    std::unique_ptr<Vec3f[]> vertices;
    uint32_t numTriangles;
    std::unique_ptr<uint32_t[]> vertexIndex;
    std::unique_ptr<Vec2f[]> stCoordinates;
};





#endif //RT_MESHTRIANGLE_H
