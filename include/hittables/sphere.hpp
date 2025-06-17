#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <optional>
#include "hit_record.hpp"
#include "interval.hpp"
#include "ray.hpp"

template <class T>
class Sphere {
 public:
  Sphere() = delete;
  Sphere(const Point3<T>& center,
         const T& radius,
         const Material_t<T>& material)
      : m_center(center), m_radius(radius), m_material(material) {};

  [[nodiscard]] auto hit(const Ray<T> ray,
                         const Interval<T>& ray_t) const noexcept
      -> const std::optional<HitRecord<T>> {
    const auto oc = m_center - ray.origin();
    const auto a = ray.direction().length_squared();
    const auto h = dot(ray.direction(), oc);
    const auto c = oc.length_squared() - m_radius * m_radius;
    const auto disciminant = h * h - a * c;
    if (disciminant < 0)
      return std::nullopt;

    const auto sqrtd = std::sqrt(disciminant);
    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      root = (h + sqrtd) / a;
      if (!ray_t.surrounds(root)) {
        return std::nullopt;
      }
    }

    const auto p = ray.at(root);
    const auto outward_normal = (p - m_center) / m_radius;
    auto hr = HitRecord<T>{.t = root, .p = p, .mat = m_material};
    hr.set_face_normal(ray, outward_normal);
    return hr;
  }

 private:
  Point3<T> m_center;
  T m_radius;
  Material_t<T> m_material;
};

#endif  // !SPHERE_HPP
