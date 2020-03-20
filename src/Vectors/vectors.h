#ifndef RT_VECTORS_H
#define RT_VECTORS_H
#include <ostream>


class Vec3 {
public:
    Vec3();
    Vec3(float xx) : x(xx), y(xx), z(xx) {}

    Vec3(float xx, float yy, float zz);
    Vec3 operator * (const float &r) const { return {x * r, y * r, z * r}; }
    Vec3 operator * (const Vec3 &v) const;
    Vec3 operator - (const Vec3 &v) const;
    Vec3 operator + (const Vec3 &v) const;
    Vec3 operator - () const;
    Vec3& operator += (const Vec3 &v);
    friend Vec3 operator * (const float &r, const Vec3 &v);
    friend std::ostream & operator << (std::ostream &os, const Vec3 &v);

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
