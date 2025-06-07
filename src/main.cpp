#include "geometry/sphere_geometry.hpp"
#include "vector3.hpp"
#include "object.hpp"

#include <iostream>

int main() {
  Vector3 vec(1, 1, 1);
  SphereGeometry sphere(vec, 1);
  Object object(std::make_unique<SphereGeometry>(sphere));

  std::cout << object.getGeometryRef().signedDistanceFrom(vec) << "\n";

  return 0;
}
