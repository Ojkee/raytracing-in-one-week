#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

template <class T>
class Ray {
 public:
  using Point3 = Vec3<T>;
  Ray() {};
  Ray(const Point3& origin, const Vec3<T>& direction)
      : m_origin(origin), m_direction(direction) {};

  [[nodiscard]] const auto& origin() const noexcept { return m_origin; }
  [[nodiscard]] const auto& direction() const noexcept { return m_direction; }

  template <class Time_t>
  [[nodiscard]] auto at(Time_t&& t) const noexcept {
    return m_origin + m_direction * t;
  }

 private:
  Point3 m_origin;
  Vec3<T> m_direction;
};

#endif  // !RAY_HPP
