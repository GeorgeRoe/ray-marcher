#pragma once

#include "src/rgb.hpp"
#include "src/types.hpp"
#include <algorithm>
#include <cstdint>

struct Color {
  Decimal r, g, b;

  Color() : r(0), g(0), b(0) {}
  Color(Decimal r_, Decimal g_, Decimal b_) : r(r_), g(g_), b(b_) {}
  Color(const RGB& channels) : r(channels.r / Decimal(255)), g(channels.g / Decimal(255)), b(channels.b / Decimal(255)) {}

  Color operator+(const Color& other) const {
    return Color(
      std::clamp<Decimal>(r + other.r, 0, 1),
      std::clamp<Decimal>(g + other.g, 0, 1),
      std::clamp<Decimal>(b + other.b, 0, 1)
    );
  }

  Color& operator+=(const Color& other) {
    r = std::clamp<Decimal>(r + other.r, 0, 1);
    g = std::clamp<Decimal>(g + other.g, 0, 1);
    b = std::clamp<Decimal>(b + other.b, 0, 1);
    return *this;
  }

  Color operator*(Decimal scalar) const {
    return Color(
      std::clamp<Decimal>(r * scalar, 0, 1),
      std::clamp<Decimal>(g * scalar, 0, 1),
      std::clamp<Decimal>(b * scalar, 0, 1)
    );
  }

  Color operator*(const Color& other) const {
    return Color(
      r * other.r,
      g * other.g,
      b * other.b
    );
  }

  RGB toRGB() const {
    return RGB(
      static_cast<uint8_t>(255 * r),
      static_cast<uint8_t>(255 * g),
      static_cast<uint8_t>(255 * b)
    );
  }
};
