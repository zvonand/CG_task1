#include "vectors.h"

Vec3::Vec3() : x(0), y(0), z(0) {}
Vec3::Vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

Vec3 Vec3::operator*(const Vec3 &v) const {
    return {x * v.x, y * v.y, z * v.z};
}
Vec3 Vec3::operator-(const Vec3 &v) const {
    return {x - v.x, y - v.y, z - v.z};
}
Vec3 Vec3::operator+(const Vec3 &v) const {
    return {x + v.x, y + v.y, z + v.z};
}
Vec3 Vec3::operator-() const {
    return Vec3(-x, -y, -z);
}
Vec3& Vec3::operator+=(const Vec3 &v) {
    x += v.x, y += v.y, z += v.z;
    return *this;
}
Vec3 operator*(const float &r, const Vec3 &v) {
    return {v.x * r, v.y * r, v.z * r};
}
std::ostream &operator<<(std::ostream &os, const Vec3 &v) {
    return os << v.x << ", " << v.y << ", " << v.z;
}


Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float xx) : x(xx), y(xx) {}
Vec2 Vec2::operator*(const float &r) const {
    return {x * r, y * r};
}

Vec2 Vec2::operator+(const Vec2 &v) const {
    return {x + v.x, y + v.y};
}
