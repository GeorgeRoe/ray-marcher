#include "camera.hpp"
#include "object.hpp"
#include "rgb.hpp"
#include "scene.hpp"
#include "src/geometry/mandelbulb_geometry.hpp"
#include "src/march_options.hpp"
#include "src/material.hpp"
#include "vector3.hpp"
#include <memory>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
  Scene scene;

  MandelbulbGeometry geom({2, 0, 0}, 1);
  Material mat({255, 0, 0}, 64, 0.8);
  Object obj(std::make_unique<MandelbulbGeometry>(geom), mat);
  scene.addOjbect(std::move(obj));

  IntegerVector2 resolution = {3840, 2560};
  Decimal aspect_ratio = static_cast<Decimal>(resolution.y) / resolution.x;

  Decimal horizontal_fov = 90 * (M_PI / 180.0);
  Decimal vertical_fov = horizontal_fov * aspect_ratio;
  DecimalVector2 fov((Decimal(horizontal_fov)), Decimal(vertical_fov));

  Camera camera({}, {1, 0, 0}, resolution, fov);

  int width = camera.getScreenSize().x;
  int height = camera.getScreenSize().y;
  int channels = 3;

  std::vector<RGB> pixels;

  MarchOptions march_options{
    1000,
    0.001,
    10000
  };

  for (Ray &ray : camera.generateRays()) {
    pixels.push_back(ray.march(scene, march_options));
  }

  stbi_write_png("output.png", width, height, channels,
                 static_cast<void *>(pixels.data()), width * channels);

  return 0;
}
