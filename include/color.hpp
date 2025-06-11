#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdint>
#include <ostream>

#include "interval.hpp"
#include "pipeline_helper.hpp"
#include "utiltools.hpp"
#include "vec3.hpp"

template <class T>
using Color = Vec3<T>;

inline auto write_color(std::ostream& out) {
  return [&out](auto pixel_color) {
    using namespace utiltools;
    using pipeline::operator|;
    static const auto interval = Interval<double>{0., 0.999};

    auto lclamp = [](const auto& v) { return interval.clamp(v); };
    auto lscale = scale(256.);
    auto lcast = clamp_cast<uint16_t>(0, 255);
    auto r = pixel_color.x() | lclamp | lscale | lcast;
    auto g = pixel_color.y() | lclamp | lscale | lcast;
    auto b = pixel_color.z() | lclamp | lscale | lcast;
    out << r << ' ' << g << ' ' << b << '\n';
  };
}

#endif  // !COLOR_HPP
