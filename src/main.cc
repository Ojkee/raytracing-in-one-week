#include "camera.hpp"
#include "color.hpp"
#include "hit_record.hpp"
#include "hittable_list.hpp"
#include "materials/dielectric.hpp"
#include "materials/lambertian.hpp"
#include "materials/metal.hpp"

auto main() -> int {
  using T = double;
  using Image_t = std::size_t;

  // MATERIALS
  Material_t<T> material_ground = Lambertian<T>{Color<T>{0.8, 0.8, 0.}};
  Material_t<T> material_center = Lambertian<T>{Color<T>{0.1, 0.2, 0.5}};
  Material_t<T> material_left = Dielectric<T>{1.5};
  Material_t<T> material_bubble = Dielectric<T>{1. / 1.5};
  Material_t<T> material_right = Metal<T>{Color<T>{0.8, 0.6, 0.2}, 0.2};

  // WORLD
  auto world = HittableList<T>{};
  world.add(Sphere{Point3<T>{0, -100.5, -1}, 100., material_ground});
  world.add(Sphere{Point3<T>{0, 0, -1.2}, 0.5, material_center});
  world.add(Sphere{Point3<T>{-1, 0, -1.0}, 0.5, material_left});
  world.add(Sphere{Point3<T>{-1, 0, -1.0}, 0.4, material_bubble});
  world.add(Sphere{Point3<T>{1, 0, -1.0}, 0.5, material_right});

  // CAMERA
  auto camera = Camera<T, Image_t>{};
  camera.render(world);

  return 0;
}
