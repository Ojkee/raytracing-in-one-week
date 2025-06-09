#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>
#include <ostream>

#include "pipeline_helper.hpp"
#include "utiltools.hpp"
#include "vec3.hpp"

template <class T>
using Color = Vec3<T>;

auto write_color(std::ostream& out) {
  return [&out](auto pixel_color) {
    using namespace utiltools;
    using pipeline::operator|;
    auto lcast = clamp_cast<uint16_t>(0, 255);
    auto lscale = scale(255.9);
    auto r = pixel_color.x() | lscale | lcast;
    auto g = pixel_color.y() | lscale | lcast;
    auto b = pixel_color.z() | lscale | lcast;
    out << r << ' ' << g << ' ' << b << '\n';
  };
}

#endif  // !COLOR_HPP
