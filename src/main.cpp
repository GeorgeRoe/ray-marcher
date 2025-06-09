#include "camera.hpp"
#include "geometry/sphere_geometry.hpp"
#include "object.hpp"
#include "rgb.hpp"
#include "scene.hpp"
#include "src/march_options.hpp"
#include "vector3.hpp"
#include <algorithm>
#include <limits>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
  Scene scene;

  DecimalVector3 vec(10, 0, 0);
  SphereGeometry sphere(vec, 2);
  Object object(std::make_unique<SphereGeometry>(sphere), {{255,0,0}});
  scene.addOjbect(std::move(object));

  SphereGeometry another_sphere({2, 1, 2}, 1);
  Object another_object(std::make_unique<SphereGeometry>(another_sphere), {{0,255,0}});
  scene.addOjbect(std::move(another_object));

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
