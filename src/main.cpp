#include "camera.hpp"
#include "geometry/sphere_geometry.hpp"
#include "object.hpp"
#include "vector3.hpp"

#include <iostream>

int main() {
  DecimalVector3 vec(10, 10, 10);
  SphereGeometry sphere(vec, 5);
  Object object(std::make_unique<SphereGeometry>(sphere));

  std::cout << "x: " << object.getGeometryRef().centroid().x << "\n";
  std::cout << "y: " << object.getGeometryRef().centroid().y << "\n";
  std::cout << "z: " << object.getGeometryRef().centroid().z << "\n";

  std::cout << "distance: " << object.getGeometryRef().signedDistanceFrom(vec)
            << "\n";

  Camera camera({}, {1, 0, 0}, {90, 45}, {800, 450});

  for (const Ray &ray : camera.generateRays()) {
    std::cout << "Ray:\n";
    std::cout << "Position:\n";
    std::cout << "x: " << ray.position.x << "\n";
    std::cout << "y: " << ray.position.y << "\n";
    std::cout << "z: " << ray.position.z << "\n";
    std::cout << "Direction:\n";
    std::cout << "x: " << ray.direction.x << "\n";
    std::cout << "y: " << ray.direction.y << "\n";
    std::cout << "z: " << ray.direction.z << "\n";
  }

  return 0;
}
