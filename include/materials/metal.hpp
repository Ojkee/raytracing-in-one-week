#ifndef METAL_HPP
#define METAL_HPP

#include <optional>
#include <utility>
#include "color.hpp"
#include "materials/material_t.hpp"
#include "ray.hpp"
#include "vec3.hpp"

template <class T>
struct HitRecord;

template <class T>
class Metal {
 public:
  Metal(const Color<T>& albedo, const T& fuzz = 0)
      : m_albedo(albedo), m_fuzz(fuzz < 1. ? fuzz : 1) {};

  [[nodiscard]] auto scatter(const Ray<T>& ray_in,
                             const HitRecord<T>& hit_record) const noexcept
      -> std::optional<ScatterData_t<T>> {
    auto reflected = reflect(ray_in.direction(), hit_record.normal);
    reflected =
        unit_vector<T>(reflected) + (m_fuzz * Vec3<T>::random_unit_vector());
    const auto scattered = Ray<T>(hit_record.p, reflected);
    const auto attenuation = m_albedo;
    return (dot(scattered.direction(), hit_record.normal) > 0.)
               ? std::optional(std::make_pair(scattered, attenuation))
               : std::nullopt;
  }

 private:
  Color<T> m_albedo{};
  T m_fuzz{};
};

#endif  // !METAL_HPP
