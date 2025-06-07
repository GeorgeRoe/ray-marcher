#pragma once

#include "types.hpp"

template <typename T>
struct Vector2 {
  T x, y;

  Vector2() : x(0), y(0) {}
  Vector2(float x_, float y_) : x(x_), y(y_) {}
};

using DecimalVector2 = Vector2<Decimal>;
using IntegerVector2 = Vector2<int>;
