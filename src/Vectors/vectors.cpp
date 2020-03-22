#include "vectors.h"

vec3::vec3() : x(0), y(0), z(0) {}
vec3::vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

vec3 vec3::operator*(const vec3 &v) const {
    return {x * v.x, y * v.y, z * v.z};
}
vec3 vec3::operator-(const vec3 &v) const {
    return {x - v.x, y - v.y, z - v.z};
}
vec3 vec3::operator+(const vec3 &v) const {
    return {x + v.x, y + v.y, z + v.z};
}
vec3 vec3::operator-() const {
    return vec3(-x, -y, -z);
}
vec3& vec3::operator+=(const vec3 &v) {
    x += v.x, y += v.y, z += v.z;
    return *this;
}
vec3 operator*(const float &r, const vec3 &v) {
    return {v.x * r, v.y * r, v.z * r};
}
std::ostream &operator<<(std::ostream &os, const vec3 &v) {
    return os << v.x << ", " << v.y << ", " << v.z;
}

float vec3::dotProduct(const vec3 &v) {
    return x * v.x + y * v.y + z * v.z;
}


Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float xx) : x(xx), y(xx) {}
Vec2 Vec2::operator*(const float &r) const {
    return {x * r, y * r};
}

Vec2 Vec2::operator+(const Vec2 &v) const {
    return {x + v.x, y + v.y};
}
