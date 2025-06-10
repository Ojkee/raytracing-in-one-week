#ifndef HIT_RECORD_HPP
#define HIT_RECORD_HPP

#include "ray.hpp"
#include "vec3.hpp"

template <class T>
struct HitRecord {
  Point3<T> p{};
  Vec3<T> normal{};
  T t{};

  bool front_face{};
  void set_face_normal(const Ray<T> ray, const Vec3<T>& outward_normal) {
    front_face = dot(ray.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

#endif  // !HIT_RECORD_HPP
