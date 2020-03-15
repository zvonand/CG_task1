#ifndef RT_VECTORS_H
#define RT_VECTORS_H
#include <ostream>


class Vec3f {
public:
    Vec3f();
    Vec3f(float xx) : x(xx), y(xx), z(xx) {}

    Vec3f(float xx, float yy, float zz);
    Vec3f operator * (const float &r) const { return {x * r, y * r, z * r}; }
    Vec3f operator * (const Vec3f &v) const;
    Vec3f operator - (const Vec3f &v) const;
    Vec3f operator + (const Vec3f &v) const;
    Vec3f operator - () const;
    Vec3f& operator += (const Vec3f &v);
    friend Vec3f operator * (const float &r, const Vec3f &v);
    friend std::ostream & operator << (std::ostream &os, const Vec3f &v);

    float x, y, z;
};

class Vec2f
{
public:
    Vec2f();
    Vec2f(float xx);
    Vec2f(float xx, float yy) : x(xx), y(yy) {}
    Vec2f operator * (const float &r) const;
    Vec2f operator + (const Vec2f &v) const;

    float x, y;
};


#endif //RT_VECTORS_H
