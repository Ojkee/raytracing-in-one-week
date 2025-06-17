#include <cmath>
#include <cstdlib>
#include "camera.hpp"
#include "color.hpp"
#include "generate_data.hpp"
#include "hit_record.hpp"
#include "hittable_list.hpp"
#include "materials/dielectric.hpp"
#include "materials/lambertian.hpp"
#include "materials/metal.hpp"
#include "ray.hpp"

auto main() -> int {
  using T = double;
  using Image_t = std::size_t;

  auto world = DataGenerator<T>().get_spheres();

  Material_t<T> ground_material = Lambertian<T>(Color<T>{0.5, 0.5, 0.5});
  world.add(Sphere<T>{Point3<T>{0, -1000, 0}, 1000, ground_material});
  Material_t<T> glass = Dielectric{1.5};
  world.add(Sphere{Point3<T>{0, 1, 0}, 1.0, glass});
  Material_t<T> matte = Lambertian{Color<T>{0.4, 0.2, 0.1}};
  world.add(Sphere{Point3<T>{-4, 1, 0}, 1.0, matte});
  Material_t<T> metal = Metal<T>{Color<T>{0.7, 0.6, 0.5}, 0.0};
  world.add(Sphere<T>{Point3<T>{4, 1, 0}, 1.0, metal});

  // CAMERA
  const auto image_width = Image_t{200};
  const auto aspect_ratio = T{16. / 9.};
  const auto samples_per_pixel = 100;
  const auto max_depth = 50;
  const auto v_fov = T{20};
  const auto lookfrom = Vec3<T>{13, 2, 3};
  const auto lookat = Vec3<T>{0, 0, 0};
  const auto v_up = Vec3<T>{0, 1, 0};
  const auto defocus_angle = T{0.6};
  const auto focus_distance = T{10};

  Camera<T, Image_t>{image_width,   aspect_ratio, samples_per_pixel,
                     max_depth,     v_fov,        lookfrom,
                     lookat,        v_up,         defocus_angle,
                     focus_distance}
      .render(world);

  return EXIT_SUCCESS;
}
