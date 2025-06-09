#pragma once

#include <cmath>
#include <type_traits>
#include "types.hpp"

template <typename T>
struct Vector3 {
  T x, y, z;

  static_assert(std::is_arithmetic<T>::value, "Vector3 requires a numeric type");

  Vector3() : x(0), y(0), z(0) {}
  Vector3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

  Vector3 operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  Vector3 operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  Vector3 operator*(T scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }

  Vector3 operator/(T scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
  }

  T dot(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  Vector3 cross(const Vector3& other) const {
    return Vector3(
      y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x
    );
  }

  Decimal length() const {
    return static_cast<Decimal>(std::sqrt(static_cast<Decimal>(x * x + y * y + z * z)));
  }

  Vector3<T> normalised() const {
    T len = length();
    return len != 0 ? *this / len : Vector3<T>();
  }
};

template <typename T>
Vector3<T> operator*(T scalar, const Vector3<T>& vec) {
  return vec * scalar;
}

using DecimalVector3 = Vector3<Decimal>;
using IntegerVector3 = Vector3<int>;
