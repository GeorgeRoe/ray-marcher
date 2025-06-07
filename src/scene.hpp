#pragma once

#include "src/object.hpp"
#include "src/vector3.hpp"
#include <algorithm>
#include <limits>
#include <vector>

class Scene {
public:
  Scene() : objects_() {}

  void addOjbect(Object object) {
    objects_.emplace_back(std::move(object));
  }

  Decimal minimumSignedDistanceFrom(DecimalVector3 point) const {
    Decimal minimum = std::numeric_limits<Decimal>::max();

    for (const Object& object : objects_) {
      minimum = std::min(object.getGeometryRef().signedDistanceFrom(point), minimum);
    }

    return minimum;
  }

private:
  std::vector<Object> objects_;
};
