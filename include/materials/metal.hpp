#ifndef METAL_HPP
#define METAL_HPP

#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

template <class T>
struct HitRecord;

template <class T>
class Metal {
 public:
  Metal(const Color<T>& albedo, const T& fuzz = 0)
      : m_albedo(albedo), m_fuzz(fuzz < 1. ? fuzz : 1) {};

  // TODO: optional tuple<hit_record, color>
  [[nodiscard]] auto scatter(const Ray<T>& ray_in,
                             const HitRecord<T>& hit_record,
                             Color<T>& attenuation,
                             Ray<T>& scattered) const noexcept -> bool {
    auto reflected = reflect(ray_in.direction(), hit_record.normal);
    reflected =
        unit_vector<T>(reflected) + (m_fuzz * Vec3<T>::random_unit_vector());
    scattered = Ray<T>(hit_record.p, reflected);
    attenuation = m_albedo;
    return (dot(scattered.direction(), hit_record.normal) > 0.);
  }

 private:
  Color<T> m_albedo{};
  T m_fuzz{};
};

#endif  // !METAL_HPP
