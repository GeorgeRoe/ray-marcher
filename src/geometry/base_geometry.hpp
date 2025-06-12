#pragma once

#include "../vector3.hpp"

class Geometry {
public:
  virtual ~Geometry() = default;
  
  virtual DecimalVector3 centroid() const = 0;
  virtual Decimal minimumSignedDistanceFrom(DecimalVector3 point) const = 0;
};
