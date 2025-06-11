#include "camera.hpp"
#include "hittable_list.hpp"

auto main() -> int {
  using Vec_t = double;
  using Image_t = std::size_t;

  // WORLD
  auto world = HittableList<Vec_t>{};
  world.add(Sphere{Point3<Vec_t>{0, 0, -1}, 0.5});
  world.add(Sphere{Point3<Vec_t>{0, -100.5, -1}, 100.});

  // CAMERA
  auto camera = Camera<Vec_t, Image_t>{};
  camera.render(world);

  return 0;
}
