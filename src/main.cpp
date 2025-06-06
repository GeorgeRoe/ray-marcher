#include "geometry/sphere_geometry.hpp"
#include "vector3.hpp"
#include <iostream>

int main() {
  Vector3 vec(1, 1, 1);
  SphereGeometry sphere(vec, 1);

  std::cout << sphere.signedDistanceFrom(vec) << "\n";

  return 0;
}
