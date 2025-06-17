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
  T t{};
  Point3<T> p{};
  Material_t<T> mat{};
  bool front_face{};
  Vec3<T> normal{};

  auto set_face_normal(const Ray<T>& ray,
                       const Vec3<T>& outward_normal) noexcept -> void {
    front_face = dot(ray.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

template <class T>
auto operator<<(std::ostream& os, const HitRecord<T>& hr) -> std::ostream& {
  os << "HitRecord {\n"
     << "  t: " << hr.t << ",\n"
     << "  p: " << hr.p << ",\n"
     << "  normal: " << hr.normal << ",\n"
     << "  front_face: " << std::boolalpha << hr.front_face << "\n"
     << "}";
  return os;
}

#endif  // !HIT_RECORD_HPP
