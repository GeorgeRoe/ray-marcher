#pragma once

#include "src/march_options.hpp"
#include "src/scene.hpp"
#include "vector3.hpp"
#include <iostream>

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

  Color march(Scene& scene, const MarchOptions march_options, int depth = 0) {
    if (depth > march_options.max_depth) {
      return Color();
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

    if (!hit) {
      return Color();
    }

    const Object& object = hit->object.get();
    const Material& material = object.getMaterial();

    if (material.emissive()) {
      return material.emission;
    }

    DecimalVector3 normal = scene.estimateNormal(getPosition());

    Color direct_light;
    for (const Object& other_object : scene.getObjects()) {

      if (!other_object.getMaterial().emissive()) {
        continue;
      }

      DecimalVector3 shadow_ray_position = position_ + normal * march_options.hit_threshold * 1.1;

      DecimalVector3 delta = other_object.getGeometryRef().centroid() - shadow_ray_position;
      DecimalVector3 direction_to_light = delta.normalised();

      bool visible = true;
      Decimal distance_to_light = delta.length();
      Ray shadow_ray(shadow_ray_position, direction_to_light);

      Decimal distance_to_travel = distance_to_light * 1.25;

      for (int steps = 0; steps < march_options.max_steps && shadow_ray.getDistanceTravelled() < distance_to_travel; ++steps) {

        auto result = scene.minimumSignedDistanceFrom(shadow_ray.getPosition());

        if (!result.has_value()) {
          break;
        } else if (result->minimum_signed_distance > march_options.hit_threshold) {
          shadow_ray.travel(result->minimum_signed_distance);
        } else if (&result->object.get() == &other_object) {
          break;
        } else {
          visible = false;
          break;
        }

      }

      if (visible) {
        Decimal intensity = std::max(Decimal(0), normal.dot(direction_to_light));
        Color addition = other_object.getMaterial().emission * intensity;
        direct_light = direct_light + addition;
      }
    }

    Color reflected_color;
    if (material.reflectivity > 0) {
      DecimalVector3 reflection_direction = direction_ - (2 * (direction_.dot(normal)) * normal);
      Ray reflected_ray(position_ + normal * march_options.hit_threshold * 1.1, reflection_direction);
      reflected_color = reflected_ray.march(scene, march_options, depth + 1);
    }

    return material.color * direct_light * (1 - material.reflectivity) + reflected_color * material.reflectivity;
  }

private:
  DecimalVector3 position_;
  DecimalVector3 direction_;
  Decimal distance_travelled_;
};
