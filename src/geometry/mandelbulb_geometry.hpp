#pragma once

#include "base_geometry.hpp"
#include <cmath>

class MandelbulbGeometry : public Geometry {
public:
  MandelbulbGeometry(DecimalVector3 center = {0, 0, 0}, Decimal scale = 1.0,
                     int max_iterations = 20, Decimal power = 8.0)
      : center_(center), scale_(scale), max_iterations_(max_iterations),
        power_(power) {}

  DecimalVector3 centroid() const override { return center_; }

  Decimal minimumSignedDistanceFrom(DecimalVector3 point) const override {
    DecimalVector3 z = (point - center_) / scale_;
    Decimal dr = 1.0;
    Decimal r = 0.0;

    for (int i = 0; i < max_iterations_; ++i) {
      r = z.length();
      if (r > 2.0)
        break;

      Decimal theta = acos(z.z / r);
      Decimal phi = atan2(z.y, z.x);
      dr = std::pow(r, power_ - 1.0) * power_ * dr + 1.0;

      Decimal zr = std::pow(r, power_);
      theta *= power_;
      phi *= power_;

      z = zr * DecimalVector3(std::sin(theta) * std::cos(phi),
                              std::sin(theta) * std::sin(phi), std::cos(theta));

      z = z + (point - center_) / scale_;
    }

    return 0.5 * std::log(r) * r / dr * scale_;
  }

private:
  DecimalVector3 center_;
  Decimal scale_;
  int max_iterations_;
  Decimal power_;
};
