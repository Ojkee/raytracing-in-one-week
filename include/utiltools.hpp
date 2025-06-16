#ifndef UTILTOOLS_HPP
#define UTILTOOLS_HPP

#include <algorithm>
#include <cmath>
#include <ranges>
#include <utility>

namespace utiltools {
template <class T>
auto scale(T&& range) {
  return [range = std::forward<T>(range)](auto&& v) { return v * range; };
}

template <class T>
decltype(auto) normalize(T&& range) {
  return [range = std::forward<T>(range)](auto&& v) { return v / (range - 1); };
}

template <class Target_t, class T1, class T2>
auto clamp_cast(T1&& min, T2&& max) {
  return [min = std::forward<T1>(min), max = std::forward<T2>(max)](auto&& v) {
    using arg_t = decltype(v);
    return static_cast<Target_t>(std::clamp(std::forward<arg_t>(v),
                                            static_cast<arg_t>(min),
                                            static_cast<arg_t>(max)));
  };
}

template <std::ranges::input_range R1, std::ranges::input_range R2>
[[nodiscard]] auto cartesian_prod(const R1& r1, const R2& r2) {
  auto inner = [=](auto x) {
    return std::views::transform(r2, [=](auto y) { return std::pair{y, x}; });
  };
  return r1 | std::views::transform(inner) | std::views::join | std::views::all;
}
}  // namespace utiltools

#endif  // !UTILTOOLS_HPP
