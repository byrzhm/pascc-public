#pragma once

#include "fmt/color.h"   // IWYU pragma: keep
#include "fmt/format.h"  // IWYU pragma: keep
#include "fmt/ranges.h"  // IWYU pragma: keep

#define LOG_ERROR(format, ...) \
  fmt::print(stderr, fmt::fg(fmt::color::red) | fmt::emphasis::bold, "[ERROR] " format "\n", __VA_ARGS__)