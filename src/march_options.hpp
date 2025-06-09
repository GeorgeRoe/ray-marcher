#pragma once

#include "src/types.hpp"

struct MarchOptions {
  int max_steps;
  Decimal hit_threshold;
  Decimal kill_threshold;
  int max_depth;
};
