#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <limits>
#include <random>
#include <type_traits>

namespace globals {
template <class T>
const T infinity = std::numeric_limits<T>::infinity();

template <class T>
const T pi = 3.1415926535897932385;

template <class T>
[[nodiscard]] inline auto degrees_to_radians(T&& degrees)
    -> std::remove_reference_t<T> {
  using U = std::remove_reference_t<T>;
  return degrees * pi<U> / 180.;
}

template <class T>
inline auto random_t(const T& min, const T& max) -> T {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<T> dist(min, max);
  return dist(gen);
}

template <class T>
inline auto random_t() -> T {
  return random_t<T>(0., 1.);
}

}  // namespace globals

#endif  // !GLOBALS_HPP
