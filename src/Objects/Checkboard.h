//
// Created by andrez on 16.03.2020.
//

#ifndef RT_CHECKBOARD_H
#define RT_CHECKBOARD_H


#include "MeshTriangle.h"

class Checkboard : public MeshTriangle{
public:
    Checkboard(const Vec3 *verts);
    Vec3 evalDiffuseColor(const Vec2 &st) const;
};



#endif //RT_CHECKBOARD_H
