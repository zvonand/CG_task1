#include "vectors.h"

Vec3f::Vec3f() : x(0), y(0), z(0) {}
Vec3f::Vec3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

Vec3f Vec3f::operator*(const Vec3f &v) const {
    return {x * v.x, y * v.y, z * v.z};
}
Vec3f Vec3f::operator-(const Vec3f &v) const {
    return {x - v.x, y - v.y, z - v.z};
}
Vec3f Vec3f::operator+(const Vec3f &v) const {
    return {x + v.x, y + v.y, z + v.z};
}
Vec3f Vec3f::operator-() const {
    return Vec3f(-x, -y, -z);
}
Vec3f& Vec3f::operator+=(const Vec3f &v) {
    x += v.x, y += v.y, z += v.z;
    return *this;
}
Vec3f operator*(const float &r, const Vec3f &v) {
    return {v.x * r, v.y * r, v.z * r};
}
std::ostream &operator<<(std::ostream &os, const Vec3f &v) {
    return os << v.x << ", " << v.y << ", " << v.z;
}


Vec2f::Vec2f() : x(0), y(0) {}
Vec2f::Vec2f(float xx) : x(xx), y(xx) {}
Vec2f Vec2f::operator*(const float &r) const {
    return {x * r, y * r};
}

Vec2f Vec2f::operator+(const Vec2f &v) const {
    return {x + v.x, y + v.y};
}
