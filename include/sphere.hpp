#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hit_record.hpp"
#include "ray.hpp"

template <class T>
class Sphere {
 public:
  Sphere() = delete;
  Sphere(const Point3<T>& center, const T& radius)
      : m_center(center), m_radius(radius) {};

  [[nodiscard]] auto hit(const Ray<T> ray,
                         const T& ray_tmin,
                         const T& ray_tmax,
                         HitRecord<T>& hit_record) -> bool;

 private:
  Point3<T> m_center;
  T m_radius;
};

#endif  // !SPHERE_HPP
