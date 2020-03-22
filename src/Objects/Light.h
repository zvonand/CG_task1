//
// Created by andrez on 14.03.2020.
//

#ifndef RT_LIGHT_H
#define RT_LIGHT_H
#include "../Vectors/vectors.h"

class Light {
public:
    Light(const vec3 &pos, const vec3 &intst) : position(pos), intensity(intst) {}
    vec3 position;
    vec3 intensity;
};


#endif //RT_LIGHT_H
