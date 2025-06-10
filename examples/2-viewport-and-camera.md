![](2-img.png)

```cpp
#include <chrono>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <ranges>
#include <utility>
#include "color.hpp"
#include "ray.hpp"
#include "utiltools.hpp"
#include "vec3.hpp"

template <class Image_t, class Ratio>
constexpr auto get_height(const Image_t& width, const Ratio& ratio)
    -> std::size_t {
  auto h = static_cast<Ratio>(width) / ratio;
  if (h < 1.0)
    h = 1.0;
  return static_cast<Image_t>(h);
}

template <class Viewport_t, class Image_t>
constexpr auto get_viewport_width(const Viewport_t& height,
                                  const Image_t& img_width,
                                  const Image_t& img_height) -> Viewport_t {
  return height * static_cast<Viewport_t>(img_width) /
         static_cast<Viewport_t>(img_height);
}

template <class T>
auto calculate_pixel_delta(T&& range) {
  return [range = std::forward<T>(range)](auto p) {
    return p / static_cast<std::remove_cvref_t<decltype(p)>::type>(range);
  };
}

template <class T>
auto make_ray_from(const Vec3<T>& pixel00_loc,
                   const Vec3<T>& pixel_delta_u,
                   const Vec3<T>& pixel_delta_v,
                   const Vec3<T>& camera_center) {
  return [&](auto pair) {
    const auto i = static_cast<T>(pair.first);
    const auto j = static_cast<T>(pair.second);
    const auto pixel_center =
        pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
    const auto ray_direction = pixel_center - camera_center;
    return Ray<T>{camera_center, ray_direction};
  };
};

template <class T>
[[nodiscard]] auto ray_color() {
  return [](auto ray) {
    auto unit_direction = unit_vector<T>(ray.direction());
    auto a = 0.5 * (unit_direction.y() + 1.);
    auto c = (1. - a) * Color<T>{1., 1., 1.} + a * Color<T>{0.5, 0.7, 1.};
    return c;
  };
}

auto main() -> int {
  using Vec_t = double;

  // IMAGE
  constexpr auto aspect_ratio = 16. / 9.;
  constexpr std::size_t img_width = 512;
  constexpr std::size_t img_height = get_height(img_width, aspect_ratio);

  // CAMERA & VIEWPORT
  constexpr auto focal_length = 1.;
  constexpr auto viewport_height = 2.;
  constexpr auto viewport_width =
      get_viewport_width(viewport_height, img_width, img_height);
  const auto camera_center = Point3<Vec_t>{0., 0., 0.};

  const auto viewport_u = Vec3<Vec_t>{viewport_width, 0, 0};
  const auto viewport_v = Vec3<Vec_t>{0, -viewport_height, 0};
  const auto pixel_delta_u = calculate_pixel_delta(img_width)(viewport_u);
  const auto pixel_delta_v = calculate_pixel_delta(img_height)(viewport_v);

  const auto viewport_upper_left = camera_center -
                                   Vec3<Vec_t>{0, 0, focal_length} -
                                   (viewport_u + viewport_v) / 2.;
  const auto pixel00_loc =
      viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

  // RENDER
  auto cout_color = write_color(std::cout);
  auto make_ray = make_ray_from<Vec_t>(pixel00_loc, pixel_delta_u,
                                       pixel_delta_v, camera_center);
  auto lray_color = ray_color<Vec_t>();

  const auto rows = std::views::iota(0u, img_height);
  const auto cols = std::views::iota(0u, img_width);

  std::clog << "===   START   ===\n" << std::flush;
  const auto start_time = std::chrono::high_resolution_clock::now();

  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
  std::ranges::for_each(utiltools::cartesian_prod(rows, cols) |
                            std::views::transform(make_ray) |
                            std::views::transform(lray_color),
                        cout_color);

  std::clog << "===   DONE    ===\n";
  const auto end_time = std::chrono::high_resolution_clock::now();
  const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);
  std::clog << "took: " << duration.count() << "ms\n";

  return 0;
}
```
