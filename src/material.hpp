#pragma once

#include "rgb.hpp"
#include "src/types.hpp"

struct Material {
  RGB color;
  Decimal reflectivity;
  Decimal specular;

  Material() = default;
  Material(const RGB& c, Decimal specular_ = 16, Decimal reflectivity_ = 0.0)
      : color(c), reflectivity(reflectivity_), specular(specular_) {}
};
