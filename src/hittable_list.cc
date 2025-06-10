#include "hittable_list.hpp"
#include <variant>
#include "hit_record.hpp"
#include "sphere.hpp"

template <class T>
auto HittableList<T>::add(const Object_t<T>& object) noexcept -> void {
  m_objects.emplace_back(object);
}

template <class T>
auto HittableList<T>::clear() noexcept -> void {
  m_objects.clear();
}

template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};
template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template <class T>
auto HittableList<T>::hit(const Ray<T>& ray,
                          const T& ray_tmin,
                          const T& ray_tmax,
                          HitRecord<T>& hit_record) noexcept -> bool {
  auto hit_rec = HitRecord<T>{};
  auto hit_anything = false;
  auto closest = ray_tmax;

  const auto hit_visitor = overloaded{
      [&](const Sphere<T>& sphere) {
        return sphere.hit(ray, ray_tmin, ray_tmax, hit_rec);
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
