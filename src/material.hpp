#pragma once

#include "color.hpp"
#include "types.hpp"

struct Material {
  Color color;
  Decimal reflectivity;
  Color emission;

  Material() = default;
  Material(const Color& color_, Decimal reflectivity_ = 0.0, const Color& emission_ = {0,0,0})
      : color(color_), reflectivity(reflectivity_), emission(emission_) {}

  bool emissive() const {
    return emission.r > 0 || emission.g > 0 || emission.b > 0;
  }
};
