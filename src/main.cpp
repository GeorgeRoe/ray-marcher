#include "geometry/sphere_geometry.hpp"
#include "camera.hpp"
#include "vector3.hpp"
#include "object.hpp"

#include <iostream>

int main() {
  Vector3 vec(10, 10, 10);
  SphereGeometry sphere(vec, 5);
  Object object(std::make_unique<SphereGeometry>(sphere));

  std::cout << "x: " << object.getGeometryRef().centroid().x << "\n";
  std::cout << "y: " << object.getGeometryRef().centroid().y << "\n";
  std::cout << "z: " << object.getGeometryRef().centroid().z << "\n";

  std::cout << "distance: " << object.getGeometryRef().signedDistanceFrom(vec) << "\n";

  Camera camera({}, {}, {}, {800, 450});

  return 0;
}
