#include "camera.hpp"
#include "geometry/sphere_geometry.hpp"
#include "object.hpp"
#include "rgb.hpp"
#include "scene.hpp"
#include "src/march_options.hpp"
#include "src/material.hpp"
#include "vector3.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
  Scene scene;

  SphereGeometry red_geom({7, 0, 0}, 1.5);
  Material red_mat({255, 0, 0}, 64, 0.8);
  Object red_sphere(std::make_unique<SphereGeometry>(red_geom), red_mat);
  scene.addOjbect(std::move(red_sphere));

  SphereGeometry green_geom({6, 2, 0}, 1);
  Material green_mat({0, 255, 0}, 8, 0.0);
  Object green_sphere(std::make_unique<SphereGeometry>(green_geom), green_mat);
  scene.addOjbect(std::move(green_sphere));

  IntegerVector2 resolution = {1920, 1080};
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
