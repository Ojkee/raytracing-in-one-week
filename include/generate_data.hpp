#ifndef GENERATE_DATA_HPP
#define GENERATE_DATA_HPP

#include "hit_record.hpp"
#include "hittable_list.hpp"
#include "vec3.hpp"

template <class T>
class DataGenerator {
 public:
  [[nodiscard]] auto get_spheres() const noexcept -> HittableList<T>;

 private:
  struct SphereData {
    Vec3<T> center{};
    Material_t<T> material{};
  };

  [[nodiscard]] auto generate_material() const noexcept;
};

template <class T>
auto DataGenerator<T>::get_spheres() const noexcept -> HittableList<T> {
  HittableList<T> world{};
  auto random_axis = [](auto&& x) {
    return static_cast<T>(x) + 0.9 * globals::random_t<T>();
  };
  auto make_center = [&random_axis](auto&& p) {
    return Point3<T>{random_axis(p.first), 0.2, random_axis(p.second)};
  };
  auto filter_center = [](const Vec3<T>& center) {
    return (center - Point3<T>{4, 0.2, 0}).length() > 0.9;
  };
  auto make_sphere = [](auto&& data) {
    return Sphere<T>{data.center, 0.2, data.material};
  };
  auto lgenerate_material = generate_material();
  auto add_sphere = [&world](auto sphere) { world.add(sphere); };

  const auto rows = std::views::iota(-11, 11);
  const auto cols = std::views::iota(-11, 11);
  std::ranges::for_each(utiltools::cartesian_prod(rows, cols) |
                            std::views::transform(make_center) |
                            std::views::filter(filter_center) |
                            std::views::transform(lgenerate_material) |
                            std::views::transform(make_sphere),
                        add_sphere);
  return world;
}

template <class T>
auto DataGenerator<T>::generate_material() const noexcept {
  return [](const Vec3<T>& center) {
    const auto r_mat = globals::random_t<T>();
    if (r_mat < 0.7) {
      const auto albedo = Color<T>::random() * Color<T>::random();
      return SphereData{.center = center, .material = Lambertian<T>{albedo}};
    } else if (r_mat < 0.9) {
      const auto albedo = Color<T>::random(0.5, 1);
      const auto fuzz = globals::random_t<T>(0, 0.5);
      return SphereData{.center = center, .material = Metal<T>{albedo, fuzz}};
    }
    return SphereData{.center = center, .material = Dielectric{0.5}};
  };
}

#endif  // !GENERATE_DATA_HPP
