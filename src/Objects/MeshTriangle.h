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
            const vec3 *verts,
            const uint32_t *vertsIndex,
            const uint32_t &numTris,
            const Vec2 *st);

    bool intersect(const vec3 &orig, const vec3 &dir, float &tnear, uint32_t &index, Vec2 &uv) const;

    void getSurfaceProperties(const vec3 &P, const vec3 &I, const uint32_t &index, const Vec2 &uv, vec3 &N, Vec2 &st) const;

    std::unique_ptr<vec3[]> vertices;
    uint32_t numTriangles;
    std::unique_ptr<uint32_t[]> vertexIndex;
    std::unique_ptr<Vec2[]> stCoordinates;

};





#endif //RT_MESHTRIANGLE_H
