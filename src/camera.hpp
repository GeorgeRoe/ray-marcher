#pragma once

#include "vector2.hpp"
#include "vector3.hpp"

class Camera {
public:
  Camera(DecimalVector3 position, DecimalVector3 direction, IntegerVector2 screen_size, DecimalVector2 fov)
      : position_(position), direction_(direction.normalised()),
        screen_size_(screen_size), fov_(fov) {}

private:
  DecimalVector3 position_;
  DecimalVector3 direction_;

  IntegerVector2 screen_size_;
  DecimalVector2 fov_;
};
