#include "../include/sphere.hpp"
#include <cmath>

template <class T>
auto Sphere<T>::hit(const Ray<T> ray,
                    const T& ray_tmin,
                    const T& ray_tmax,
                    HitRecord<T>& hit_record) -> bool {
  const auto oc = m_center - ray.origin();
  const auto a = ray.direction().length_squared();
  const auto h = dot(ray.direction(), oc);
  const auto c = oc.length_squared() - m_radius * m_radius;
  const auto disciminant = h * h - a * c;
  if (disciminant < 0)
    return false;

  const auto sqrtd = std::sqrt(disciminant);
  const auto root = (h - sqrtd) / a;
  if (root <= ray_tmin || ray_tmax <= root) {
    root = (h + sqrtd) / a;
    if (root <= ray_tmin || ray_tmax)
      return false;
  }

  hit_record.t = root;
  hit_record.p = ray.at(hit_record.t);
  const auto outward_normal = (hit_record.p - m_center) / m_radius;
  hit_record.set_face_normal(ray, outward_normal);

  return true;
}
