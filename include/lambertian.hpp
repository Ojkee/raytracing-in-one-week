#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "color.hpp"
#include "ray.hpp"

template <class T>
struct HitRecord;

template <class T>
class Lambertian {
 public:
  Lambertian(const Color<T>& albedo) : m_albedo(albedo) {};

  // TODO: optional tuple<hit_record, color>
  [[nodiscard]] auto scatter([[maybe_unused]] const Ray<T>& ray_in,
                             const HitRecord<T>& hit_record,
                             Color<T>& attenuation,
                             Ray<T>& scattered) const noexcept -> bool {
    const auto scatter_direction =
        hit_record.normal + Vec3<T>::random_unit_vector();
    if (scatter_direction.near_zero())
      scatter_direction = hit_record.normal;

    scattered = Ray<T>(hit_record.p, scatter_direction);
    attenuation = m_albedo;
    return true;
  }

 private:
  Color<T> m_albedo{};
};

#endif  // !LAMBERTIAN_HPP
