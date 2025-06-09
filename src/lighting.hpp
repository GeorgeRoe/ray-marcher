#pragma once

#include "vector3.hpp"
#include "rgb.hpp"
#include "material.hpp"

namespace Lighting {
  RGB applyLighting(const DecimalVector3& position, const DecimalVector3& normal,
                  const DecimalVector3& view_dir, const Material& mat) {
    DecimalVector3 light_dir = (DecimalVector3(10, 10, -10) - position).normalised();
    Decimal diff = std::max(Decimal(0), normal.dot(light_dir));

    DecimalVector3 reflect_dir = 2 * normal.dot(light_dir) * normal - light_dir;
    Decimal spec = std::pow(std::max(view_dir.dot(reflect_dir), Decimal(0)), mat.specular);

    RGB light_color(255, 255, 255);
    RGB diffuse = mat.color * diff;
    RGB specular = light_color * spec;

    return (diffuse + specular) * 0.2;
  }
}
