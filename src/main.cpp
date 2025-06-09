#include "camera.hpp"
#include "geometry/sphere_geometry.hpp"
#include "object.hpp"
#include "rgb.hpp"
#include "scene.hpp"
#include "vector3.hpp"
#include <algorithm>
#include <limits>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

int main() {
  Scene scene;

  DecimalVector3 vec(10, 0, 0);
  SphereGeometry sphere(vec, 2);
  Object object(std::make_unique<SphereGeometry>(sphere));
  scene.addOjbect(std::move(object));

  SphereGeometry another_sphere({2, 1, 2}, 1);
  Object another_object(std::make_unique<SphereGeometry>(another_sphere));
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

  Decimal max_distance = 10000;
  Decimal min_distance = 0.00001;

  for (Ray &ray : camera.generateRays()) {
    RGB color;
    int steps = 0;
    Decimal closest_distance = std::numeric_limits<Decimal>::max();
    while (ray.getDistanceTravelled() < max_distance) {
      Decimal minimum_signed_distance =
          scene.minimumSignedDistanceFrom(ray.getPosition());

      closest_distance = std::max(Decimal(0), std::min(minimum_signed_distance, closest_distance));

      if (minimum_signed_distance < min_distance) {
        color.r = std::max(255 - steps * 5, 0);
        break;
      }

      ray.march(minimum_signed_distance);
      steps++;
    }

    Decimal glow_radius = 0.2;
    closest_distance = std::min(glow_radius, std::max(Decimal(0), closest_distance));
    Decimal glow_color = 255 * (Decimal(1) - closest_distance / glow_radius);
    color.b = std::max(0, std::min(int(glow_color), 255));
    pixels.push_back(color);
  }

  stbi_write_png("output.png", width, height, channels,
                 static_cast<void *>(pixels.data()), width * channels);

  return 0;
}
