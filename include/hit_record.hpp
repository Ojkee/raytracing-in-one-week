#ifndef HIT_RECORD_HPP
#define HIT_RECORD_HPP

#include <variant>
#include "materials/dielectric.hpp"
#include "materials/lambertian.hpp"
#include "materials/metal.hpp"
#include "ray.hpp"
#include "vec3.hpp"

template <class T>
using Material_t =
    std::variant<std::monostate, Lambertian<T>, Metal<T>, Dielectric<T>>;

template <class T>
struct HitRecord {
  Point3<T> p{};
  Vec3<T> normal{};
  Material_t<T> mat{};
  T t{};
  bool front_face{};

  auto set_face_normal(const Ray<T>& ray,
                       const Vec3<T>& outward_normal) noexcept -> void {
    front_face = dot(ray.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

#endif  // !HIT_RECORD_HPP
