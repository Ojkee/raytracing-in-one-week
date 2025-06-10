#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

template <class T>
using Point3 = Vec3<T>;

template <class T>
class Ray {
 public:
  Ray() {};
  Ray(const Point3<T>& origin, const Vec3<T>& direction)
      : m_origin(origin), m_direction(direction) {};

  [[nodiscard]] auto origin() const noexcept -> const Point3<T>& {
    return m_origin;
  }
  [[nodiscard]] auto direction() const noexcept -> const Vec3<T>& {
    return m_direction;
  }

  template <class Time_t>
  [[nodiscard]] auto at(Time_t&& t) const noexcept -> Point3<T> {
    return m_origin + m_direction * t;
  }

 private:
  Point3<T> m_origin;
  Vec3<T> m_direction;
};

#endif  // !RAY_HPP
