#pragma once

#include "geometry.hpp"

class SphereGeometry : public Geometry {
public:
  SphereGeometry() : center_(), radius_(1) {}
  SphereGeometry(Vector3 center, float radius) : center_(center), radius_(radius) {}

  Vector3 centroid() const override {
    return center_;
  }

  float signedDistanceFrom(Vector3 point) const override {
    return (point - center_).length() - radius_;
  }

private:
  Vector3 center_;
  float radius_;
};
