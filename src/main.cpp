#include "camera.hpp"
#include "object.hpp"
#include "rgb.hpp"
#include "scene.hpp"
#include "src/geometry/sphere_geometry.hpp"
#include "src/geometry/mandelbulb_geometry.hpp"
#include "src/march_options.hpp"
#include "src/material.hpp"
#include "vector3.hpp"
#include <cstdlib>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <thread>
#include <utility>

int main() {
  Scene scene;

  scene.addObject(Object(
    std::make_unique<MandelbulbGeometry>(DecimalVector3(3, 0, 0), 1, 20, 8),
    Material(RGB(255, 0, 200), 0, {0, 0, 0})
  ));

  scene.addObject(Object(
    std::make_unique<SphereGeometry>(DecimalVector3(1, 2, 0), 0.5),
    Material({0, 0, 0}, 0.0, RGB(255, 255, 255))
  ));

  IntegerVector2 resolution = {80, 80};
  int scale = 20;
  resolution.x *= scale;
  resolution.y *= scale;
  Decimal aspect_ratio = static_cast<Decimal>(resolution.y) / resolution.x;

  Decimal horizontal_fov = 50 * (M_PI / 180.0);
  Decimal vertical_fov = horizontal_fov * aspect_ratio;
  DecimalVector2 fov((Decimal(horizontal_fov)), Decimal(vertical_fov));

  Camera camera({}, {1, 0, 0}, resolution, fov);

  MarchOptions march_options{
    1000,
    0.0001,
    10000,
    100
  };

  auto rays = camera.generateRays();

  std::vector<RGB> pixels(rays.size());

  int n = std::thread::hardware_concurrency();

  Decimal chunk_size = rays.size() / Decimal(n);

  std::vector<size_t> starting_indexes = {};
  for (Decimal i = 0; i < rays.size(); i += chunk_size) {
    starting_indexes.push_back(static_cast<size_t>(i));
  }

  std::vector<size_t> ending_indexes = {};
  for (size_t i = 0; i < starting_indexes.size(); ++i) {
    if (i == 0) continue;

    ending_indexes.push_back(starting_indexes.at(i) - 1);
  }
  ending_indexes.push_back(rays.size() - 1);

  std::vector<std::pair<size_t, size_t>> indexes;
  for (size_t i = 0; i < starting_indexes.size(); ++i) {
    indexes.push_back({starting_indexes.at(i), ending_indexes.at(i)});
  }

  srand(0);
  std::vector<size_t> randomness(pixels.size());
  for (size_t i = 0; i < pixels.size(); ++i) {
    size_t random = rand() % pixels.size();

    randomness.at(i) = random;
    randomness.at(random) = i;
  }

  std::vector<std::thread> threads;
  for (const auto& index_bounds: indexes) {
    threads.emplace_back([&]() {
      for (size_t i = index_bounds.first; i <= index_bounds.second; ++i) {
        pixels.at(randomness.at(i)) = rays.at(randomness.at(i)).march(scene, march_options).toRGB();
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }

  int width = camera.getScreenSize().x;
  int height = camera.getScreenSize().y;
  int channels = 3;

  stbi_write_png("output.png", width, height, channels,
                 static_cast<void *>(pixels.data()), width * channels);

  return 0;
}
