#pragma once

#include "../vector3.hpp"

class Geometry {
public:
  virtual ~Geometry() {}
  
  virtual Vector3 centroid() const = 0;
  virtual float signedDistanceFrom(Vector3 point) const = 0;
};
