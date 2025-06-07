#pragma once

#include <cmath>

struct Vector3 {
  float x, y, z;

  Vector3() : x(0), y(0), z(0) {}
  Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

  Vector3 operator+(const Vector3& other) const {
      return Vector3(x + other.x, y + other.y, z + other.z);
  }

  Vector3 operator-(const Vector3& other) const {
      return Vector3(x - other.x, y - other.y, z - other.z);
  }

  Vector3 operator*(float scalar) const {
      return Vector3(x * scalar, y * scalar, z * scalar);
  }

  Vector3 operator/(float scalar) const {
      return Vector3(x / scalar, y / scalar, z / scalar);
  }

  float dot(const Vector3& other) const {
      return x * other.x + y * other.y + z * other.z;
  }

  Vector3 cross(const Vector3& other) const {
      return Vector3(
          y * other.z - z * other.y,
          z * other.x - x * other.z,
          x * other.y - y * other.x
      );
  }

  float length() const {
      return std::sqrt(x * x + y * y + z * z);
  }

  Vector3 normalized() const {
      float len = length();
      return len != 0 ? *this / len : Vector3();
  }
};
