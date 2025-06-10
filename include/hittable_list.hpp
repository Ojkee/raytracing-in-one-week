#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <variant>
#include <vector>
#include "hit_record.hpp"
#include "ray.hpp"
#include "sphere.hpp"

template <class T>
using Object_t = std::variant<Sphere<T>>;

template <class T>
class HittableList {
 public:
  HittableList<T>(){};
  HittableList<T>(const Object_t<T>& object) { add(object); };

  auto add(const Object_t<T>& object) noexcept -> void;
  auto clear() noexcept -> void;
  [[nodiscard]] auto hit(const Ray<T>& ray,
                         const T& ray_tmin,
                         const T& ray_tmax,
                         HitRecord<T>& hit_record) noexcept -> bool;

 private:
  std::vector<Object_t<T>> m_objects{};
};

#endif  // !HITTABLE_LIST_HPP
