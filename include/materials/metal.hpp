#ifndef METAL_HPP
#define METAL_HPP

#include "color.hpp"
#include "ray.hpp"

template <class T>
struct HitRecord;

template <class T>
class Metal {
 public:
  Metal(const Color<T>& albedo) : m_albedo(albedo) {};

  // TODO: optional tuple<hit_record, color>
  [[nodiscard]] auto scatter(const Ray<T>& ray_in,
                             const HitRecord<T>& hit_record,
                             Color<T>& attenuation,
                             Ray<T>& scattered) const noexcept -> bool {
    const auto reflected =
        Vec3<T>::reflect(ray_in.direction(), hit_record.normal);
    scattered = Ray<T>(hit_record.p, reflected);
    attenuation = m_albedo;
    return true;
  }

 private:
  Color<T> m_albedo{};
};

#endif  // !METAL_HPP
