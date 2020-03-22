#ifndef RT_VECTORS_H
#define RT_VECTORS_H
#include <ostream>


class vec3 {
public:
    vec3();
    vec3(float xx) : x(xx), y(xx), z(xx) {}

    vec3(float xx, float yy, float zz);
    vec3 operator * (const float &r) const { return {x * r, y * r, z * r}; }
    vec3 operator * (const vec3 &v) const;
    vec3 operator - (const vec3 &v) const;
    vec3 operator + (const vec3 &v) const;
    vec3 operator - () const;
    vec3& operator += (const vec3 &v);
    float dotProduct(const vec3 &v);

    friend vec3 operator * (const float &r, const vec3 &v);
    friend std::ostream & operator << (std::ostream &os, const vec3 &v);


    float x, y, z;
};

class Vec2
{
public:
    Vec2();
    Vec2(float xx);
    Vec2(float xx, float yy) : x(xx), y(yy) {}
    Vec2 operator * (const float &r) const;
    Vec2 operator + (const Vec2 &v) const;

    float x, y;
};


#endif //RT_VECTORS_H
