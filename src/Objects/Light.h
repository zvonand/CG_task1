//
// Created by andrez on 14.03.2020.
//

#ifndef RT_LIGHT_H
#define RT_LIGHT_H
#include "../Vectors/vectors.h"

class Light {
public:
    Light(const Vec3f &pos, const Vec3f &intst) : position(pos), intensity(intst) {}
    Vec3f position;
    Vec3f intensity;
};


#endif //RT_LIGHT_H
