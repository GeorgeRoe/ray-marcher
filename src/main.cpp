#include "camera.hpp"
#include "object.hpp"
#include "rgb.hpp"
#include "scene.hpp"
#include "src/geometry/sphere_geometry.hpp"
#include "src/geometry/mandelbulb_geometry.hpp"
#include "src/march_options.hpp"
#include "src/material.hpp"
#include "vector3.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
  Scene scene;

  scene.addObject(Object(
    std::make_unique<MandelbulbGeometry>(DecimalVector3(7, 0, -1.5), 1),
    Material(RGB(255, 100, 100), 0.25, {0, 0, 0})
  ));

  scene.addObject(Object(
    std::make_unique<SphereGeometry>(DecimalVector3(7, 0, 1.5), 1),
    Material(RGB(100, 100, 255), 0, {0, 0, 0})
  ));

  scene.addObject(Object(
    std::make_unique<SphereGeometry>(DecimalVector3(5, 2, 0), 1),
    Material({0, 0, 0}, 0.0, RGB(255, 255, 255))
  ));

  scene.addObject(Object(
    std::make_unique<SphereGeometry>(DecimalVector3(5, -2, 0), 1),
    Material({0, 0, 0}, 0.0, RGB(255, 255, 255))
  ));

  IntegerVector2 resolution = {80, 45};
  int scale = 100;
  resolution.x *= scale;
  resolution.y *= scale;
  Decimal aspect_ratio = static_cast<Decimal>(resolution.y) / resolution.x;

  Decimal horizontal_fov = 90 * (M_PI / 180.0);
  Decimal vertical_fov = horizontal_fov * aspect_ratio;
  DecimalVector2 fov((Decimal(horizontal_fov)), Decimal(vertical_fov));

  Camera camera({2, 0, 0}, {1, 0, 0}, resolution, fov);

  int width = camera.getScreenSize().x;
  int height = camera.getScreenSize().y;
  int channels = 3;

  std::vector<RGB> pixels;

  MarchOptions march_options{
    1000,
    0.001,
    10000,
    100
  };

  for (Ray &ray : camera.generateRays()) {
    pixels.push_back(ray.march(scene, march_options).toRGB());
  }

  stbi_write_png("output.png", width, height, channels,
                 static_cast<void *>(pixels.data()), width * channels);

  return 0;
}
