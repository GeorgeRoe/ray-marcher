#pragma once

#include "geometry/mandelbulb_geometry.hpp"
#include "geometry/plane_geometry.hpp"
#include "geometry/sphere_geometry.hpp"
#include "geometry/triangle_geometry.hpp"
#include <variant>

using GeometryVariant = std::variant<
  MandelbulbGeometry,
  PlaneGeometry,
  SphereGeometry,
  TriangleGeometry
>;

class GeometryVisitor {
public:
  static DecimalVector3 getCentroid(const GeometryVariant& geometry) {
    return std::visit([&](const auto& visiting_geometry) {
      return visiting_geometry.centroid();
    }, geometry);
  }

  static Decimal getMinimumSignedDistanceFrom(const GeometryVariant& geometry, const DecimalVector3& point) {
    return std::visit([&](const auto& visiting_geometry) {
      return visiting_geometry.minimumSignedDistanceFrom(point);
    }, geometry);
  }
};
