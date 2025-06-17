#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include <optional>
#include <utility>
#include "color.hpp"
#include "materials/material_t.hpp"
#include "ray.hpp"

template <class T>
struct HitRecord;

template <class T>
class Lambertian {
 public:
  Lambertian(const Color<T>& albedo) : m_albedo(albedo) {};

  [[nodiscard]] auto scatter([[maybe_unused]] const Ray<T>& ray_in,
                             const HitRecord<T>& hit_record) const noexcept
      -> std::optional<ScatterData_t<T>> {
    auto scatter_direction = hit_record.normal + Vec3<T>::random_unit_vector();
    if (scatter_direction.near_zero())
      scatter_direction = hit_record.normal;

    const auto ray = Ray<T>{hit_record.p, scatter_direction};
    return std::optional(std::make_pair(ray, m_albedo));
  }

 private:
  Color<T> m_albedo{};
};

#endif  // !LAMBERTIAN_HPP
