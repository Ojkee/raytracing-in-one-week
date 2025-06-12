#ifndef SPHERE_HPP
#define SPHERE_HPP

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

  // TODO: optional HitRecord
  [[nodiscard]] auto hit(const Ray<T> ray,
                         const Interval<T>& ray_t,
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
    if (!ray_t.surrounds(root)) {
      root = (h + sqrtd) / a;
      if (!ray_t.surrounds(root))
        return false;
    }

    hit_record.t = root;
    hit_record.p = ray.at(hit_record.t);
    const auto outward_normal = (hit_record.p - m_center) / m_radius;
    hit_record.set_face_normal(ray, outward_normal);
    hit_record.mat = m_material;

    return true;
  }

 private:
  Point3<T> m_center;
  T m_radius;
  Material_t<T> m_material;
};

#endif  // !SPHERE_HPP
