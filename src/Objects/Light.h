//
// Created by andrez on 14.03.2020.
//

#ifndef RT_LIGHT_H
#define RT_LIGHT_H
#include "../Vectors/vectors.h"

class Light {
public:
    Light(const Vec3 &pos, const Vec3 &intst) : position(pos), intensity(intst) {}
    Vec3 position;
    Vec3 intensity;
};


#endif //RT_LIGHT_H
