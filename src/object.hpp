#pragma once

#include "geometry/geometry.hpp"
#include "src/material.hpp"
#include <memory>

class Object {
public:
  Object(std::unique_ptr<Geometry> geometry, Material material) : geometry_(std::move(geometry)), material_(material) {}

  const Geometry& getGeometryRef() const {
    return *geometry_;
  }

  const Material getMaterial() const {
    return material_;
  }

private:
  std::unique_ptr<Geometry> geometry_;
  Material material_;
};
