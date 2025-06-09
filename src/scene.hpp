#pragma once

#include "src/object.hpp"
#include "src/vector3.hpp"
#include <functional>
#include <limits>
#include <optional>
#include <vector>

struct SceneQueryResult {
  std::reference_wrapper<Object> object;
  Decimal minimum_signed_distance;
};

class Scene {
public:
  Scene() : objects_() {}

  void addOjbect(Object object) {
    objects_.emplace_back(std::move(object));
  }

  std::vector<SceneQueryResult> objectsMinimumSignedDistancesFrom(DecimalVector3 point) {
    std::vector<SceneQueryResult> results;

    for (Object& object : objects_) {
      Decimal distance = object.getGeometryRef().minimumSignedDistanceFrom(point);
      results.push_back({object, distance});
    }

    return results;
  }

  std::optional<SceneQueryResult> minimumSignedDistanceFrom(DecimalVector3 point) {
    std::optional<SceneQueryResult> closest_result;
    Decimal closest_distance = std::numeric_limits<Decimal>::max();

    for (const SceneQueryResult& result : objectsMinimumSignedDistancesFrom(point)) {
      if (result.minimum_signed_distance < closest_distance) {
        closest_distance = result.minimum_signed_distance;
        closest_result = result;
      }
    }

    return closest_result;
  }

  DecimalVector3 estimateNormal(const DecimalVector3& point) {
    auto closest = minimumSignedDistanceFrom(point);

    Decimal distance = closest.has_value() ? closest.value().minimum_signed_distance : std::numeric_limits<Decimal>::max();

    Decimal epsilon = std::min(distance * 0.001, Decimal(1e-6));

    Decimal dx = minimumSignedDistanceFrom(point + DecimalVector3(epsilon, 0, 0))->minimum_signed_distance -
                minimumSignedDistanceFrom(point - DecimalVector3(epsilon, 0, 0))->minimum_signed_distance;
    Decimal dy = minimumSignedDistanceFrom(point + DecimalVector3(0, epsilon, 0))->minimum_signed_distance -
                minimumSignedDistanceFrom(point - DecimalVector3(0, epsilon, 0))->minimum_signed_distance;
    Decimal dz = minimumSignedDistanceFrom(point + DecimalVector3(0, 0, epsilon))->minimum_signed_distance -
                minimumSignedDistanceFrom(point - DecimalVector3(0, 0, epsilon))->minimum_signed_distance;

    return DecimalVector3(dx, dy, dz).normalised();
  }

private:
  std::vector<Object> objects_;
};
