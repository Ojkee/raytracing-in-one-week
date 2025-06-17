#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <iostream>
#include <optional>
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
                         const Interval<T>& ray_t) const noexcept
      -> const std::optional<HitRecord<T>>;

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
                          const Interval<T>& ray_t) const noexcept
    -> const std::optional<HitRecord<T>> {
  const auto make_hit_visitor = [&ray](const auto& closest) {
    return overloaded{
        [ray, closest](const Sphere<T>& sphere) {
          return sphere.hit(ray, closest);
        },
    };
  };

  struct Acc {
    std::optional<HitRecord<T>> hr;
    double c;
  };

  auto hit_closest = [&ray_t, &make_hit_visitor](auto&& acc, const auto& obj) {
    const auto interval = Interval<T>(ray_t.min(), acc.c);
    const auto hr = std::visit(make_hit_visitor(interval), obj);
    return hr ? Acc{.hr = hr, .c = hr->t} : acc;
  };

  const auto init = Acc{.hr = std::nullopt, .c = ray_t.max()};
  const auto res =
      std::ranges::fold_left(m_objects, std::move(init), hit_closest);
  return res.hr;
}

#endif  // !HITTABLE_LIST_HPP
