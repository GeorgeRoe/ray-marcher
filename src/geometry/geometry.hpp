#pragma once

#include "../vector3.hpp"

class Geometry {
public:
  virtual ~Geometry() = default;
  
  const virtual Vector3 centroid() const = 0;
  const virtual float signedDistanceFrom(Vector3 point) const = 0;
};
