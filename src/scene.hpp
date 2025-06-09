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

private:
  std::vector<Object> objects_;
};
