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

  auto add(const Object_t<T>& object) noexcept -> void {
    m_objects.emplace_back(object);
  }
  auto clear() noexcept -> void { m_objects.clear(); }
  [[nodiscard]] auto hit(const Ray<T>& ray,
                         const T& ray_tmin,
                         const T& ray_tmax,
                         HitRecord<T>& hit_record) const noexcept -> bool {
    auto hit_rec = HitRecord<T>{};
    auto hit_anything = false;
    auto closest = ray_tmax;

    const auto hit_visitor = overloaded{
        [&](const Sphere<T>& sphere) {
          return sphere.hit(ray, ray_tmin, closest, hit_rec);
        },
    };

    for (const auto& object : m_objects) {
      if (std::visit(hit_visitor, object)) {
        hit_anything = true;
        closest = hit_rec.t;
        hit_record = hit_rec;
      }
    }
    return hit_anything;
  }

 private:
  template <typename... Ts>
  struct overloaded : Ts... {
    using Ts::operator()...;
  };
  template <typename... Ts>
  overloaded(Ts...) -> overloaded<Ts...>;

  std::vector<Object_t<T>> m_objects{};
};

#endif  // !HITTABLE_LIST_HPP
