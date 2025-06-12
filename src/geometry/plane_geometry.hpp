#pragma once

#include "base_geometry.hpp"

class PlaneGeometry : public Geometry {
public:
  PlaneGeometry() : normal_(DecimalVector3(0, 1, 0)), point_(DecimalVector3(0, 0, 0)) {}
  PlaneGeometry(DecimalVector3 normal, DecimalVector3 point) : normal_(normal.normalised()), point_(point) {}

  DecimalVector3 centroid() const override {
    return point_;
  }

  Decimal minimumSignedDistanceFrom(DecimalVector3 point) const override {
    return (point - point_).dot(normal_);
  }

private:
  DecimalVector3 normal_;
  DecimalVector3 point_;
};

