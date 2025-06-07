#include "camera.hpp"
#include "geometry/sphere_geometry.hpp"
#include "object.hpp"
#include "scene.hpp"
#include "vector3.hpp"
#include "rgb.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

int main() {
  DecimalVector3 vec(10, 0, 0);
  SphereGeometry sphere(vec, 2);
  Object object(std::make_unique<SphereGeometry>(sphere));
  Scene scene;

  scene.addOjbect(std::move(object));

  Camera camera({}, {1, 0, 0}, {800, 450}, {90, 45});

  std::cout << "distance: "
            << scene.minimumSignedDistanceFrom(camera.getPosition()) << "\n";

  int width = camera.getScreenSize().x;
  int height = camera.getScreenSize().y;
  int channels = 3;

  std::vector<RGB> pixels(width * height);

  std::srand(0);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int i = y * width + x;
      pixels[i] = RGB(
        std::rand() % 256,
        std::rand() % 256,
        std::rand() % 256
      );
    }
  }

  stbi_write_png("output.png", width, height, channels,
                 static_cast<void*>(pixels.data()), width * channels);

  return 0;
}
