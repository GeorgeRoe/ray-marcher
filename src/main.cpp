#include "vector3.hpp"
#include <iostream>

int main() {
  Vector3 a(1.0f, 0.0f, 0.0f);
  Vector3 b(0.0f, 1.0f, 0.0f);

  std::cout << "a dot b = " << a.dot(b) << "\n";

  return 0;
}
