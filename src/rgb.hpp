#pragma once

#include "src/types.hpp"
#include <algorithm>
#include <cstdint>

struct RGB {
  uint8_t r, g, b;

  RGB() : r(0), g(0), b(0) {}
  RGB(uint8_t r_, uint8_t g_, uint8_t b_) : r(r_), g(g_), b(b_) {}

  RGB operator+(const RGB& other) const {
    return RGB(
      std::clamp<int>(r + other.r, 0, 255),
      std::clamp<int>(g + other.g, 0, 255),
      std::clamp<int>(b + other.b, 0, 255)
    );
  }

  RGB& operator+=(const RGB& other) {
    r = std::clamp<int>(r + other.r, 0, 255);
    g = std::clamp<int>(g + other.g, 0, 255);
    b = std::clamp<int>(b + other.b, 0, 255);
    return *this;
  }

  RGB operator*(Decimal scalar) const {
    return RGB(
      std::clamp(int(r * scalar), 0, 255),
      std::clamp(int(g * scalar), 0, 255),
      std::clamp(int(b * scalar), 0, 255)
    );
  }

  RGB operator*(const RGB& other) const {
    return RGB(
      (r * other.r) / 255,
      (g * other.g) / 255,
      (b * other.b) / 255
    );
  }
};
