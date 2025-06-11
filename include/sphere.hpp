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
                         HitRecord<T>& hit_record) const noexcept -> bool {
    const auto oc = m_center - ray.origin();
    const auto a = ray.direction().length_squared();
    const auto h = dot(ray.direction(), oc);
    const auto c = oc.length_squared() - m_radius * m_radius;
    const auto disciminant = h * h - a * c;
    if (disciminant < 0)
      return false;

    const auto sqrtd = std::sqrt(disciminant);
    auto root = (h - sqrtd) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
      root = (h + sqrtd) / a;
      if (root <= ray_tmin || ray_tmax <= root)
        return false;
    }

    hit_record.t = root;
    hit_record.p = ray.at(hit_record.t);
    const auto outward_normal = (hit_record.p - m_center) / m_radius;
    hit_record.set_face_normal(ray, outward_normal);

    return true;
  }

 private:
  Point3<T> m_center;
  T m_radius;
};

#endif  // !SPHERE_HPP
