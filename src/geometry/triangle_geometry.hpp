#pragma once

#include "geometry.hpp"
#include <algorithm>

class TriangleGeometry : public Geometry {
public:
  TriangleGeometry() : a_(), b_(), c_() {}
  TriangleGeometry(DecimalVector3 a, DecimalVector3 b, DecimalVector3 c)
      : a_(a), b_(b), c_(c) {}

  DecimalVector3 centroid() const override {
    return (a_ + b_ + c_) / 3.0;
  }

  Decimal minimumSignedDistanceFrom(DecimalVector3 point) const override {
    return pointToTriangleSDF(point, a_, b_, c_);
  }

private:
  DecimalVector3 a_, b_, c_;

  static Decimal pointToTriangleSDF(const DecimalVector3& p, const DecimalVector3& a,
                                    const DecimalVector3& b, const DecimalVector3& c) {
    const DecimalVector3 ab = b - a;
    const DecimalVector3 bc = c - b;
    const DecimalVector3 ca = a - c;

    const DecimalVector3 pa = p - a;
    const DecimalVector3 pb = p - b;
    const DecimalVector3 pc = p - c;

    const DecimalVector3 n = (b - a).cross(c - a).normalised();

    Decimal dist_to_plane = pa.dot(n);

    Decimal inside = 1.0;
    inside *= ((ab).cross(pa)).dot(n) >= 0 ? 1.0 : 0.0;
    inside *= ((bc).cross(pb)).dot(n) >= 0 ? 1.0 : 0.0;
    inside *= ((ca).cross(pc)).dot(n) >= 0 ? 1.0 : 0.0;

    if (inside > 0.5) {
      return std::abs(dist_to_plane);
    }

    Decimal d_ab = pointToSegmentDistance(p, a, b);
    Decimal d_bc = pointToSegmentDistance(p, b, c);
    Decimal d_ca = pointToSegmentDistance(p, c, a);

    return std::min<Decimal>({d_ab, d_bc, d_ca});
  }

  static Decimal pointToSegmentDistance(const DecimalVector3& p,
                                        const DecimalVector3& a,
                                        const DecimalVector3& b) {
    DecimalVector3 ab = b - a;
    Decimal t = ((p - a).dot(ab)) / ab.dot(ab);
    t = std::max<Decimal>(0.0, std::min<Decimal>(1.0, t));
    DecimalVector3 closest = a + ab * t;
    return (p - closest).length();
  }
};
