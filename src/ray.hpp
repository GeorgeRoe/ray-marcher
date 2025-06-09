#pragma once

#include "src/march_options.hpp"
#include "src/rgb.hpp"
#include "src/scene.hpp"
#include "vector3.hpp"

class Ray {
public:
  Ray(const DecimalVector3 &position, const DecimalVector3 &direction)
      : position_(position), direction_(direction.normalised()), distance_travelled_(0) {}

  void travel(Decimal distance) {
    position_ = position_ + (direction_ * distance);
    distance_travelled_ += distance;
  }

  DecimalVector3 getPosition() const {
    return position_;
  }
  
  DecimalVector3 getDirection() const {
    return direction_;
  }

  Decimal getDistanceTravelled() const {
    return distance_travelled_;
  }

  RGB march(Scene& scene, MarchOptions march_options) {
    std::optional<SceneQueryResult> hit;

    for (int steps = 0; steps < march_options.max_steps && getDistanceTravelled() < march_options.kill_threshold; ++steps) {
      std::optional<SceneQueryResult> result = scene.minimumSignedDistanceFrom(getPosition());     

      if (result.has_value()) {
        if (result->minimum_signed_distance < march_options.hit_threshold) {
          hit = result;
          break;
        }

        travel(result->minimum_signed_distance);
      } else {
        travel(std::numeric_limits<Decimal>::max());
      }
    }


    return hit.has_value() ? hit->object.get().getMaterial().color : RGB();
  }

private:
  DecimalVector3 position_;
  DecimalVector3 direction_;
  Decimal distance_travelled_;
};
