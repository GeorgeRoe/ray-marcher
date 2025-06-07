#pragma once

#include "vector2.hpp"
#include "vector3.hpp"

class Camera {
public:
  Camera(Vector3 position, Vector3 direction, Vector2 screen_size, Vector2 fov)
      : position_(position), direction_(direction.normalized()),
        screen_size_(screen_size), fov_(fov) {}

private:
  Vector3 position_;
  Vector3 direction_;

  Vector2 screen_size_;
  Vector2 fov_;
};
