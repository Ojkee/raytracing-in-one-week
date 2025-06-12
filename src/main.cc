#include "camera.hpp"
#include "color.hpp"
#include "hit_record.hpp"
#include "hittable_list.hpp"
#include "materials/lambertian.hpp"
#include "materials/metal.hpp"

auto main() -> int {
  using Vec_t = double;
  using Image_t = std::size_t;

  // MATERIALS
  Material_t<Vec_t> material_ground =
      Lambertian<Vec_t>{Color<Vec_t>{0.8, 0.8, 0.}};
  Material_t<Vec_t> material_center =
      Lambertian<Vec_t>{Color<Vec_t>{0.1, 0.2, 0.5}};
  Material_t<Vec_t> material_left = Metal<Vec_t>{Color<Vec_t>{0.8, 0.8, 0.8}};
  Material_t<Vec_t> material_right = Metal<Vec_t>{Color<Vec_t>{0.8, 0.6, 0.2}};

  // WORLD
  auto world = HittableList<Vec_t>{};
  world.add(Sphere{Point3<Vec_t>{0, -100.5, -1}, 100., material_ground});
  world.add(Sphere{Point3<Vec_t>{0, 0, -1.2}, 0.5, material_center});
  world.add(Sphere{Point3<Vec_t>{-1, 0, -1.0}, 0.5, material_left});
  world.add(Sphere{Point3<Vec_t>{1, 0, -1.0}, 0.5, material_right});

  // CAMERA
  auto camera = Camera<Vec_t, Image_t>{};
  camera.render(world);

  return 0;
}
