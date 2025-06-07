#pragma once

#include "geometry/geometry.hpp"
#include <memory>

class Object {
public:
  Object(std::unique_ptr<Geometry> geometry) : geometry_(std::move(geometry)) {}

  const Geometry& getGeometryRef() const {
    return *geometry_;
  }

private:
  std::unique_ptr<Geometry> geometry_;
};
