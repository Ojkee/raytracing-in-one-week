#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <limits>

namespace globals {
template <class T>
const T infinity = std::numeric_limits<T>::infinity();

template <class T>
const T pi = 3.1415926535897932385;

template <class T>
[[nodiscard]] inline auto degrees_to_radians(T&& degrees) -> T {
  return degrees * pi<T> / 180.;
}

template <class Image_t, class Ratio>
constexpr auto get_height(const Image_t& width, const Ratio& ratio) -> Image_t {
  auto h = static_cast<Ratio>(width) / ratio;
  if (h < 1.0)
    h = 1.0;
  return static_cast<Image_t>(h);
}
}  // namespace globals

#endif  // !GLOBALS_HPP
