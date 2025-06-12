#pragma once

#include "geometry.hpp"
#include "src/material.hpp"

class Object {
public:
  Object(GeometryVariant geometry, Material material)
      : geometry_(geometry), material_(material) {}

  Object(Object &&) = default;
  Object &operator=(Object &&) = default;

  Object(const Object &) = delete;
  Object &operator=(const Object &) = delete;

  const GeometryVariant& getGeometry() const { return geometry_; }

  const Material& getMaterial() const { return material_; }

private:
  GeometryVariant geometry_;
  Material material_;
};
