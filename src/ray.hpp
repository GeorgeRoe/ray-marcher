#pragma once

#include "vector3.hpp"

struct Ray {
  DecimalVector3 position;
  DecimalVector3 direction;

  Ray(const DecimalVector3 &position_, const DecimalVector3 &direction_)
      : position(position_), direction(direction_.normalised()) {}
};
