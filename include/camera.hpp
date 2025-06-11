#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cerrno>
#include <chrono>
#include <iostream>
#include "color.hpp"
#include "globals.hpp"
#include "hittable_list.hpp"
#include "viewport.hpp"

template <class T, class Image_t>
class Camera {
 public:
  Camera()
      : m_aspect_ratio(16. / 9.),
        m_img_width(400),
        m_img_height(globals::get_height(m_img_width, m_aspect_ratio)),
        m_camera_center(Point3<T>{0, 0, 0}),
        m_viewport(Viewport<T>(m_camera_center, m_img_width, m_img_height)) {};

  auto render(const HittableList<T>& world) const noexcept -> void {
    auto cout_color = write_color(std::cout);
    auto make_ray = m_viewport.ray_generator();
    auto lray_color = ray_color(world);

    const auto rows = std::views::iota(0u, m_img_height);
    const auto cols = std::views::iota(0u, m_img_width);

    std::clog << "===   START   ===\n" << std::flush;
    const auto start_time = std::chrono::high_resolution_clock::now();

    std::cout << "P3\n" << m_img_width << ' ' << m_img_height << "\n255\n";
    std::ranges::for_each(utiltools::cartesian_prod(rows, cols) |
                              std::views::transform(make_ray) |
                              std::views::transform(lray_color),
                          cout_color);

    std::clog << "===   DONE    ===\n";
    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    std::clog << "took: " << duration.count() << "ms\n";
  }

 private:
  [[nodiscard]] auto ray_color(const HittableList<T>& world) const noexcept {
    return [&world](auto ray) {
      HitRecord<T> hit_record;
      if (world.hit(ray, Interval{0., globals::infinity<T>}, hit_record)) {
        return 0.5 * (hit_record.normal + Color<T>{1, 1, 1});
      }

      auto unit_direction = unit_vector<T>(ray.direction());
      auto a = 0.5 * (unit_direction.y() + 1.);
      auto c = (1. - a) * Color<T>{1., 1., 1.} + a * Color<T>{0.5, 0.7, 1.};
      return c;
    };
  }

  const T m_aspect_ratio{};
  Image_t m_img_width{};
  Image_t m_img_height{};
  Point3<T> m_camera_center{};
  Viewport<T> m_viewport{};
};

#endif  // !CAMERA_HPP
