#include <algorithm>
#include <cmath>
#include <ranges>
#include "camera.hpp"
#include "color.hpp"
#include "globals.hpp"
#include "hit_record.hpp"
#include "hittable_list.hpp"
#include "materials/dielectric.hpp"
#include "materials/lambertian.hpp"
#include "materials/metal.hpp"
#include "ray.hpp"
#include "utiltools.hpp"

auto main() -> int {
  using T = double;
  using Image_t = std::size_t;

  HittableList<T> world{};

  Material_t<T> ground_material = Lambertian<T>(Color<T>{0.5, 0.5, 0.5});
  world.add(Sphere<T>{Point3<T>{0, -1000, 0}, 1000, ground_material});

  auto rows = std::views::iota(-11, 11);
  auto cols = std::views::iota(-11, 11);

  auto random_axis = [](auto x) {
    return static_cast<T>(x) + 0.9 * globals::random_t<T>();
  };
  auto make_center = [&random_axis](auto p) {
    return Point3<T>{random_axis(p.first), 0.2, random_axis(p.second)};
  };
  auto filter_center = [](Vec3<T> center) {
    return (center - Point3<T>{4, 0.2, 0}).length() > 0.9;
  };
  auto generate_material =
      [](auto center) -> const std::tuple<Material_t<T>, decltype(center)> {
    const auto r_mat = globals::random_t<T>();
    if (r_mat < 0.7) {
      const auto albedo = Color<T>::random() * Color<T>::random();
      return {Lambertian<T>{albedo}, center};
    } else if (r_mat < 0.9) {
      const auto albedo = Color<T>::random(0.5, 1);
      const auto fuzz = globals::random_t<T>(0, 0.5);
      return {Metal<T>{albedo, fuzz}, center};
    }
    return {Dielectric{0.5}, center};
  };
  auto make_sphere = [](auto mat_center) {
    return Sphere<T>{std::get<1>(mat_center), 0.2, std::get<0>(mat_center)};
  };
  auto add_sphere = [&world](auto sphere) { world.add(sphere); };

  std::ranges::for_each(utiltools::cartesian_prod(rows, cols) |
                            std::views::transform(make_center) |
                            std::views::filter(filter_center) |
                            std::views::transform(generate_material) |
                            std::views::transform(make_sphere),
                        add_sphere);

  Material_t<T> glass = Dielectric{1.5};
  world.add(Sphere{Point3<T>{0, 1, 0}, 1.0, glass});
  Material_t<T> matte = Lambertian{Color<T>{0.4, 0.2, 0.1}};
  world.add(Sphere{Point3<T>{-4, 1, 0}, 1.0, matte});
  Material_t<T> metal = Metal<T>{Color<T>{0.7, 0.6, 0.5}, 0.0};
  world.add(Sphere<T>{Point3<T>{4, 1, 0}, 1.0, metal});

  // CAMERA
  const auto image_width = Image_t{1000};
  const auto aspect_ratio = T{16. / 9.};
  const auto samples_per_pixel = 200;
  const auto max_depth = 50;
  const auto v_fov = T{20};
  const auto lookfrom = Vec3<T>{13, 2, 3};
  const auto lookat = Vec3<T>{0, 0, 0};
  const auto v_up = Vec3<T>{0, 1, 0};
  const auto defocus_angle = T{0.6};
  const auto focus_distance = T{10};

  auto camera =
      Camera<T, Image_t>{image_width,   aspect_ratio, samples_per_pixel,
                         max_depth,     v_fov,        lookfrom,
                         lookat,        v_up,         defocus_angle,
                         focus_distance};
  camera.render(world);

  return 0;
}
