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

#include <thread>
#include <utility>

int main() {
  Scene scene;

  scene.addObject(Object(
    std::make_unique<MandelbulbGeometry>(DecimalVector3(3, 0, 0), 1, 100, 15),
    Material(RGB(255, 255, 255), 0, {0, 0, 0})
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
  int scale = 3;
  resolution.x *= scale;
  resolution.y *= scale;
  Decimal aspect_ratio = static_cast<Decimal>(resolution.y) / resolution.x;

  Decimal horizontal_fov = 50 * (M_PI / 180.0);
  Decimal vertical_fov = horizontal_fov * aspect_ratio;
  DecimalVector2 fov((Decimal(horizontal_fov)), Decimal(vertical_fov));

  Camera camera({}, {1, 0, 0}, resolution, fov);

  MarchOptions march_options{
    10000,
    0.001,
    15,
    6000 
  };

  auto rays = camera.generateRays();

  const size_t total_pixels = rays.size();
  std::vector<RGB> pixels(total_pixels);

  const int total_threads = std::thread::hardware_concurrency();
  std::cout << "Using " << total_threads << " threads.\n";

  size_t drawn_pixels = 0;
  
  std::cout << "Progress: 0%\n";

  std::vector<std::thread> threads;
  for (int thread_index = 0; thread_index < total_threads; ++thread_index) {
    threads.emplace_back([thread_index, &rays, &pixels, &scene, &march_options, total_threads, total_pixels, &drawn_pixels]() {
      bool in_bounds = true;
      size_t leap_counter = 0;
      while (in_bounds) {
        size_t pixel_index = leap_counter * total_threads + thread_index;

        if (pixel_index >= pixels.size()) {
          in_bounds = false;
        } else {
          pixels.at(pixel_index) = rays.at(pixel_index).march(scene, march_options).toRGB();
          drawn_pixels++;
    
          if (drawn_pixels % (total_pixels / 100) == 0) {
            size_t progress = drawn_pixels * 100 / total_pixels;
            std::cout << "Progress: " << progress << "%\n";
          }
        }

        leap_counter++;
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }

  std::cout << "final image written\n";

  const int width = camera.getScreenSize().x;
  const int height = camera.getScreenSize().y;
  const int channels = 3;

  stbi_write_png("output.png", width, height, channels,
                 static_cast<void *>(pixels.data()), width * channels);

  return 0;
}
