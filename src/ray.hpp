#pragma once

#include "src/lighting.hpp"
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

  RGB marchOld(Scene& scene, MarchOptions march_options) {
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

  RGB march(Scene& scene, MarchOptions march_options, int depth = 0) {
    if (depth > march_options.max_depth) {
      return RGB();
    }

    std::optional<SceneQueryResult> hit;

    for (int steps = 0; steps < march_options.max_steps && getDistanceTravelled() < march_options.kill_threshold; ++steps) {
      auto result = scene.minimumSignedDistanceFrom(getPosition());

      if (result && result->minimum_signed_distance < march_options.hit_threshold) {
        hit = result;
        break;
      }

      travel(result ? result->minimum_signed_distance : std::numeric_limits<Decimal>::max());
    }

    if (!hit) return RGB();

    const Object& object = hit->object.get();
    const Material& material = object.getMaterial();

    DecimalVector3 normal = scene.estimateNormal(getPosition());

    RGB lighting = Lighting::applyLighting(getPosition(), normal, direction_ * -1, material);

    RGB reflected_color;
    if (material.reflectivity > 0) {
      DecimalVector3 reflection_direction = direction_ - (2 * (direction_.dot(normal)) * normal);
      Ray reflected_ray(position_ + normal * 0.0001, reflection_direction);
      reflected_color = reflected_ray.march(scene, march_options, depth + 1);
    }

    return material.color * (1 - material.reflectivity) + reflected_color * material.reflectivity + lighting;
  }

private:
  DecimalVector3 position_;
  DecimalVector3 direction_;
  Decimal distance_travelled_;
};
