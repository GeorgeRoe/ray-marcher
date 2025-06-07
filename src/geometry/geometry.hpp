#pragma once

#include "../vector3.hpp"

class Geometry {
public:
  virtual ~Geometry() = default;
  
  virtual DecimalVector3 centroid() const = 0;
  virtual Decimal signedDistanceFrom(DecimalVector3 point) const = 0;
};
