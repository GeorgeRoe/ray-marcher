#pragma once

#include "base_geometry.hpp"

class SphereGeometry : public Geometry {
public:
  SphereGeometry() : center_(), radius_(1) {}
  SphereGeometry(DecimalVector3 center, float radius) : center_(center), radius_(radius) {}

  DecimalVector3 centroid() const override {
    return center_;
  }

  Decimal minimumSignedDistanceFrom(DecimalVector3 point) const override {
    return (point - center_).length() - radius_;
  }

private:
  DecimalVector3 center_;
  Decimal radius_;
};
