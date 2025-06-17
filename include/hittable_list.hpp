#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <variant>
#include <vector>
#include "hit_record.hpp"
#include "hittables/sphere.hpp"
#include "interval.hpp"
#include "ray.hpp"

template <class T>
using Hittable_t = std::variant<Sphere<T>>;

template <class T>
class HittableList {
 public:
  HittableList() {};

  auto add(const Hittable_t<T>& object) noexcept -> void;
  auto clear() noexcept -> void;
  [[nodiscard]] auto hit(const Ray<T>& ray,
                         const Interval<T>& ray_t,
                         HitRecord<T>& hit_record) const noexcept -> bool;

 private:
  std::vector<Hittable_t<T>> m_objects{};
};

template <class T>
auto HittableList<T>::add(const Hittable_t<T>& object) noexcept -> void {
  m_objects.emplace_back(object);
}

template <class T>
auto HittableList<T>::clear() noexcept -> void {
  m_objects.clear();
}

template <class T>
auto HittableList<T>::hit(const Ray<T>& ray,
                          const Interval<T>& ray_t,
                          HitRecord<T>& hit_record) const noexcept -> bool {
  auto hit_anything = false;
  auto closest = ray_t.max();

  const auto hit_visitor = overloaded{
      [&](const Sphere<T>& sphere) {
        const auto closest_interval = Interval{ray_t.min(), closest};
        return sphere.hit(ray, closest_interval);
      },
  };

  for (const auto& object : m_objects) {
    if (auto hr = std::visit(hit_visitor, object); hr) {
      hit_anything = true;
      closest = hr.value().t;
      hit_record = hr.value();
    }
  }

  return hit_anything;
}

#endif  // !HITTABLE_LIST_HPP
