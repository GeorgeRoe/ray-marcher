#include "camera.hpp"
#include "geometry/sphere_geometry.hpp"
#include "object.hpp"
#include "src/scene.hpp"
#include "vector3.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

int main() {
  DecimalVector3 vec(10, 10, 10);
  SphereGeometry sphere(vec, 5);
  Object object(std::make_unique<SphereGeometry>(sphere));
  Scene scene;

  scene.addOjbect(std::move(object));

  std::cout << "distance: " << scene.minimumSignedDistanceFrom(vec) << "\n";

  Camera camera({}, {1, 0, 0}, {800, 450}, {90, 45});

  int width = camera.getScreenSize().x;
  int height = camera.getScreenSize().y;
  int channels = 3;
  std::vector<unsigned char> pixels(width * height * channels);

  std::srand(1);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int i = (y * width + x) * channels;
      pixels[i + 0] = std::rand() % 256; // R
      pixels[i + 1] = std::rand() % 256; // G
      pixels[i + 2] = std::rand() % 256; // B
    }
  }

  stbi_write_png("output.png", width, height, channels, pixels.data(),
                 width * channels);

  return 0;
}
