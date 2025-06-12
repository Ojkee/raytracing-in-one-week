#ifndef COLOR_HPP
#define COLOR_HPP

#include <cmath>
#include <cstdint>
#include <functional>
#include <ostream>
#include <type_traits>
#include <utility>

#include "fn_cpp_helper.hpp"
#include "interval.hpp"
#include "utiltools.hpp"
#include "vec3.hpp"

template <class T>
using Color = Vec3<T>;

template <class T>
inline auto linear_to_gamma(T&& linear_component) noexcept
    -> std::remove_reference_t<T> {
  using V_t = std::remove_reference_t<T>;
  const V_t zero = T{0.};
  return (linear_component > zero) ? std::sqrt(linear_component) : zero;
}

inline auto write_color(std::ostream& out) {
  return [&out](auto pixel_color) {
    using namespace utiltools;
    using pipeline::operator|;
    static const auto interval = Interval<double>{0., 0.999};

    auto lclamp = [](const auto& v) { return interval.clamp(v); };
    auto lscale = scale(256.);
    auto lcast = clamp_cast<uint16_t>(0, 255);
    auto lgamma = [](auto&& v) {
      using V_t = decltype(v);
      return std::invoke(linear_to_gamma<V_t>, std::forward<V_t>(v));
    };
    auto r = pixel_color.x() | lgamma | lclamp | lscale | lcast;
    auto g = pixel_color.y() | lgamma | lclamp | lscale | lcast;
    auto b = pixel_color.z() | lgamma | lclamp | lscale | lcast;
    out << r << ' ' << g << ' ' << b << '\n';
  };
}

#endif  // !COLOR_HPP
