#pragma once

#include <vector>

#include "ray.hpp"
#include "vector2.hpp"
#include "vector3.hpp"

class Camera {
public:
  Camera(DecimalVector3 position, DecimalVector3 direction,
         IntegerVector2 screen_size, DecimalVector2 fov)
      : position_(position), direction_(direction.normalised()),
        screen_size_(screen_size), fov_(fov) {}

  std::vector<Ray> generateRays() const {
    std::vector<Ray> rays;
    rays.reserve(screen_size_.x * screen_size_.y);

    Decimal half_width = std::tan(fov_.x * static_cast<Decimal>(0.5));
    Decimal half_height = std::tan(fov_.y * static_cast<Decimal>(0.5));

    Decimal pixel_width = (2 * half_width) / screen_size_.x;
    Decimal pixel_height = (2 * half_height) / screen_size_.y;

    DecimalVector3 forward = direction_;
    DecimalVector3 right = forward.cross(DecimalVector3(0, 1, 0)).normalised();
    DecimalVector3 up = right.cross(forward).normalised();

    for (int y = 0; y < screen_size_.y; ++y) {
      for (int x = 0; x < screen_size_.x; ++x) {
        Decimal px = (x + 0.5f) * pixel_width - half_width;
        Decimal py = half_height - (y + 0.5f) * pixel_height;

        DecimalVector3 ray_dir = (forward + right * px + up * py).normalised();
        rays.emplace_back(position_, ray_dir);
      }
    }

    return rays;
  }

  DecimalVector3 getPosition() const {
    return position_;
  }

  DecimalVector3 getDirection() const {
    return direction_;
  }

  IntegerVector2 getScreenSize() const {
    return screen_size_;
  }

  DecimalVector2 getFov() const {
    return fov_;
  }

private:
  DecimalVector3 position_;
  DecimalVector3 direction_;

  IntegerVector2 screen_size_;
  DecimalVector2 fov_;
};
