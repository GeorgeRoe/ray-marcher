#include "camera.hpp"
#include "object.hpp"
#include "rgb.hpp"
#include "scene.hpp"
#include "src/geometry/sphere_geometry.hpp"
#include "src/geometry/plane_geometry.hpp"
#include "src/march_options.hpp"
#include "src/material.hpp"
#include "vector3.hpp"
#include <atomic>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <thread>
#include <iostream>

int main() {
  Scene scene;

  scene.addObject(Object(
    SphereGeometry(DecimalVector3(5, 0, 0), 1),
    Material({1,1,1}, 0.35, RGB(0, 0, 0))
  ));
  scene.addObject(Object(
    SphereGeometry(DecimalVector3(2, 1.5, 0), 0.5),
    Material({}, 0, RGB(255, 255, 255))
  ));

  const Decimal wall_reflectivity = 0.8;

  scene.addObject(Object(
    PlaneGeometry(DecimalVector3(-1, 0, 0), DecimalVector3(10, 0, 0)),
    Material(RGB(255, 0, 0), wall_reflectivity, {})
  ));
  scene.addObject(Object(
    PlaneGeometry(DecimalVector3(1, 0, 0), DecimalVector3(-1, 0, 0)),
    Material(RGB(255, 0, 0), wall_reflectivity, {})
  ));

  scene.addObject(Object(
    PlaneGeometry(DecimalVector3(0, 0, -1), DecimalVector3(0, 0, 10)),
    Material(RGB(0, 255, 0), wall_reflectivity, {})
  ));
  scene.addObject(Object(
    PlaneGeometry(DecimalVector3(0, 0, 1), DecimalVector3(0, 0, -10)),
    Material(RGB(0, 255, 0), wall_reflectivity, {})
  ));

  scene.addObject(Object(
    PlaneGeometry(DecimalVector3(0, -1, 0), DecimalVector3(0, 5, 0)),
    Material(RGB(0, 0, 255), wall_reflectivity, {})
  ));
  scene.addObject(Object(
    PlaneGeometry(DecimalVector3(0, 1, 0), DecimalVector3(0, -5, 0)),
    Material(RGB(0, 0, 255), wall_reflectivity, {})
  ));

  Decimal aspect_ratio = 1.78;  
  int output_width = 400;
  int fov_degrees = 120;

  IntegerVector2 resolution(output_width, static_cast<int>(output_width / aspect_ratio));

  Decimal horizontal_fov = fov_degrees * (Decimal(M_PI) / Decimal(180));
  DecimalVector2 fov(horizontal_fov, horizontal_fov / aspect_ratio);

  Camera camera({}, {1, 0, 0}, resolution, fov);

  MarchOptions march_options{
    1000,
    0.001,
    1000,
    500 
  };

  auto rays = camera.generateRays();

  const size_t total_pixels = rays.size();
  std::vector<RGB> pixels(total_pixels);

  const int total_threads = std::thread::hardware_concurrency();
  std::cout << "Using " << total_threads << " threads.\n";

  std::atomic<size_t> drawn_pixels(0);
  
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
          drawn_pixels.fetch_add(1, std::memory_order_relaxed);
    
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
