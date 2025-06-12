#pragma once

#include "ray.hpp"
#include "vector2.hpp"
#include "vector3.hpp"

class Camera {
public:
  Camera(DecimalVector3 position, DecimalVector3 direction,
         IntegerVector2 screen_size, DecimalVector2 fov)
      : position_(position), forward_(direction.normalised()),
        right_(forward_.cross(DecimalVector3(0, 1, 0)).normalised()),
        up_(right_.cross(forward_).normalised()), screen_size_(screen_size),
        fov_(fov),
        half_screen_size_(std::tan(fov.x * static_cast<Decimal>(0.5)),
                          std::tan(fov.y * static_cast<Decimal>(0.5))),
        pixel_((2 * half_screen_size_.x) / screen_size.x,
               (2 * half_screen_size_.y) / screen_size.y) {}

  Ray generateRay(const int& x, const int& y) const {
    Decimal px = (x + Decimal(0.5)) * pixel_.x - half_screen_size_.x;
    Decimal py = half_screen_size_.y - (y + Decimal(0.5)) * pixel_.y;
    DecimalVector3 ray_direction = (forward_ + right_ * px + up_ * py).normalised();
    return Ray(position_, ray_direction);
  }

  Ray generateRay(const IntegerVector2& pixel) const {
    return generateRay(pixel.x, pixel.y);
  }

  IntegerVector2 indexToCoordinate(const size_t& index) const {
    return IntegerVector2(
      static_cast<int>(index % screen_size_.x),
      static_cast<int>(index / screen_size_.x)
    );
  }

private:
  DecimalVector3 position_;

  DecimalVector3 forward_;
  DecimalVector3 right_;
  DecimalVector3 up_;

  IntegerVector2 screen_size_;
  DecimalVector2 fov_;

  DecimalVector2 half_screen_size_;
  DecimalVector2 pixel_;
};
