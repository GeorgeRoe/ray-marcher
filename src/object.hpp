#pragma once

#include "geometry/geometry.hpp"
#include "src/material.hpp"
#include <memory>

class Object {
public:
  Object(std::unique_ptr<Geometry> geometry, Material material)
      : geometry_(std::move(geometry)), material_(material) {}

  Object(Object &&) = default;
  Object &operator=(Object &&) = default;

  Object(const Object &) = delete;
  Object &operator=(const Object &) = delete;

  const Geometry &getGeometryRef() const { return *geometry_; }

  const Material getMaterial() const { return material_; }

private:
  std::unique_ptr<Geometry> geometry_;
  Material material_;
};
