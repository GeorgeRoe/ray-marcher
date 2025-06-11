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
    std::make_unique<MandelbulbGeometry>(DecimalVector3(3, 0, 0), 1, 100, 15),
    Material(RGB(255, 255, 255), 0.4, {0, 0, 0})
  ));

  scene.addObject(Object(
    std::make_unique<SphereGeometry>(DecimalVector3(0, 1, 1), 0.5),
    Material({0, 0, 0}, 0.0, RGB(0, 255, 255))
  ));
  scene.addObject(Object(
    std::make_unique<SphereGeometry>(DecimalVector3(0, 1, -1), 0.5),
    Material({0, 0, 0}, 0.0, RGB(255, 0, 255))
  ));

  IntegerVector2 resolution = {80, 80};
  int scale = 1;
  resolution.x *= scale;
  resolution.y *= scale;
  Decimal aspect_ratio = static_cast<Decimal>(resolution.y) / resolution.x;

  Decimal horizontal_fov = 50 * (M_PI / 180.0);
  Decimal vertical_fov = horizontal_fov * aspect_ratio;
  DecimalVector2 fov((Decimal(horizontal_fov)), Decimal(vertical_fov));

  Camera camera({}, {1, 0, 0}, resolution, fov);

  MarchOptions march_options{
    10000000,
    0.0001,
    15,
    5000 
  };

  auto rays = camera.generateRays();

  std::vector<RGB> pixels(rays.size());

  int total_threads = std::thread::hardware_concurrency() - 4;
  std::cout << "Using " << total_threads << " threads.\n";

  Decimal chunk_size = rays.size() / Decimal(total_threads);

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
    randomness.at(i) = i;
  }

  for (size_t i = 0; i < pixels.size(); ++i) {
    size_t random = rand() % pixels.size();

    size_t temp = randomness.at(i);
    randomness.at(i) = randomness.at(random);
    randomness.at(random) = temp;
  }

  size_t processed_rays = 0;
  const size_t total_rays = rays.size();
  std::cout << "Progress: 0%\n";

  int width = camera.getScreenSize().x;
  int height = camera.getScreenSize().y;
  int channels = 3;

  size_t write_frequency = 10;

  std::vector<std::thread> threads;
  for (const auto& index_bounds: indexes) {
    threads.emplace_back([&]() {
      for (size_t i = index_bounds.first; i <= index_bounds.second; ++i) {
        pixels.at(randomness.at(i)) = rays.at(randomness.at(i)).march(scene, march_options).toRGB();
        processed_rays++;

        if (processed_rays % (total_rays / 100) == 0) {
          size_t progress = processed_rays * 100 / total_rays;
          std::cout << "Progress: " << progress << "%\n";
          if (progress % write_frequency == 0) {
            std::cout << "image written\n";
            stbi_write_png(std::format("output-{}.png", processed_rays * 100 / total_rays).c_str(), width, height, channels,
                           static_cast<void *>(pixels.data()), width * channels);
          }
        }
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }
  std::cout << "\rProgress: 100%\n";

  std::cout << "final image written\n";
  stbi_write_png("output.png", width, height, channels,
                 static_cast<void *>(pixels.data()), width * channels);

  return 0;
}
