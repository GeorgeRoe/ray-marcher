#pragma once

#include "vector3.hpp"

class Ray {
public:
  Ray(const DecimalVector3 &position, const DecimalVector3 &direction)
      : position_(position), direction_(direction.normalised()), distance_travelled_(0) {}

  void march(Decimal distance) {
    position_ = position_ + (direction_ * distance);
    distance_travelled_ += distance;
  }

  DecimalVector3 getPosition() const {
    return position_;
  }
  
  DecimalVector3 getDirection() const {
    return direction_;
  }

  Decimal getDistanceTravelled() const {
    return distance_travelled_;
  }

private:
  DecimalVector3 position_;
  DecimalVector3 direction_;
  Decimal distance_travelled_;
};
