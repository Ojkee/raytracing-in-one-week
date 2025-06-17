#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include <algorithm>
#include <cmath>
#include <optional>
#include <utility>
#include "color.hpp"
#include "globals.hpp"
#include "materials/material_t.hpp"
#include "ray.hpp"
#include "vec3.hpp"

template <class T>
struct HitRecord;

template <class T>
class Dielectric {
 public:
  Dielectric() : m_refraction_index(1.5) {};
  Dielectric(const T& refraction_index)
      : m_refraction_index(refraction_index) {};

  [[nodiscard]] auto scatter(const Ray<T>& ray_in,
                             const HitRecord<T>& hit_record) const noexcept
      -> std::optional<ScatterData_t<T>> {
    const auto attenuation = Color<T>{1., 1., 1.};
    const T ri =
        hit_record.front_face ? (1. / m_refraction_index) : m_refraction_index;

    const auto unit_direction = unit_vector(ray_in.direction());
    const auto cos_theta =
        std::min<T>(dot(-unit_direction, hit_record.normal), 1.);
    const T sin_theta = std::sqrt(1. - cos_theta * cos_theta);

    const auto cannot_refract = ri * sin_theta > 1.;

    auto direction = Vec3<T>{};
    if (cannot_refract || reflectance(cos_theta, ri) > globals::random_t<T>())
      direction = reflect(unit_direction, hit_record.normal);
    else
      direction = refract(unit_direction, hit_record.normal, ri);

    const auto scattered = Ray<T>{hit_record.p, direction};
    return std::optional(std::make_pair(scattered, attenuation));
  }

 private:
  [[nodiscard]] static auto reflectance(const T& cos,
                                        const T& refraction_index) noexcept
      -> T;

  T m_refraction_index{};
};

template <class T>
auto Dielectric<T>::reflectance(const T& cos,
                                const T& refraction_index) noexcept -> T {
  auto r0 = (1 - refraction_index) / (1 + refraction_index);
  r0 *= r0;
  return r0 + (1 - r0) * std::pow((1 - cos), 5.);
}

#endif  // !DIELECTRIC_HPP
