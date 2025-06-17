#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <variant>
#include "color.hpp"
#include "fn_cpp_helper.hpp"
#include "globals.hpp"
#include "hit_record.hpp"
#include "hittable_list.hpp"
#include "materials/material_t.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "viewport.hpp"

template <class T, class Image_t>
class Camera {
 public:
  Camera<T, Image_t>(const Image_t& image_width,
                     const T& aspect_ratio,
                     const std::size_t& samples_per_pixel,
                     const int& max_depth,
                     const T& v_fov,
                     const Vec3<T>& lookfrom,
                     const Vec3<T>& lookat,
                     const Vec3<T>& v_up,
                     const T& defocus_angle,
                     const T& focus_distance)
      : m_aspect_ratio(aspect_ratio),
        m_img_width(image_width),
        m_img_height(get_height(m_img_width, m_aspect_ratio)),

        m_samples_per_pixel(samples_per_pixel),
        m_pixel_samples_scale(1. / static_cast<T>(m_samples_per_pixel)),

        m_v_fov(v_fov),
        m_focal_lenght((lookfrom - lookat).length()),
        m_theta(globals::degrees_to_radians(m_v_fov)),
        m_h(std::tan(m_theta / 2.)),
        m_viewport(Viewport<T>(m_img_width,
                               m_img_height,
                               2 * m_h * focus_distance,
                               lookfrom,
                               lookat,
                               v_up,
                               defocus_angle,
                               focus_distance)),
        m_max_depth(max_depth) {}

  auto render(const HittableList<T>& world) const noexcept -> void;

 private:
  template <class Ratio>
  [[nodiscard]] constexpr auto get_height(const Image_t& width,
                                          const Ratio& ratio) -> const Image_t;
  [[nodiscard]] auto ray_color(const Ray<T>& ray,
                               const int depth,
                               const HittableList<T>& world) const noexcept
      -> Color<T>;
  [[nodiscard]] auto backgound_color(const Vec3<T>& direction) const noexcept
      -> const Color<T>;
  [[nodiscard]] auto get_ray() const noexcept;
  [[nodiscard]] auto sample_square() const noexcept -> Vec3<T>;
  [[nodiscard]] auto defocus_disk_sample() const noexcept -> const Point3<T>;
  [[nodiscard]] auto material_scatter(
      const Ray<T>& ray,
      const HitRecord<T>& hit_record) const noexcept;

  const T m_aspect_ratio{};
  const Image_t m_img_width{};
  const Image_t m_img_height{};
  const std::size_t m_samples_per_pixel{};
  const T m_pixel_samples_scale{};
  const T m_v_fov{};
  const T m_focal_lenght{};
  const T m_theta{};
  const T m_h{};
  const Viewport<T> m_viewport{};
  const int m_max_depth{};
};

template <class T, class Image_t>
template <class Ratio>
constexpr auto Camera<T, Image_t>::get_height(const Image_t& width,
                                              const Ratio& ratio)
    -> const Image_t {
  auto h = static_cast<Ratio>(width) / ratio;
  if (h < 1.0)
    h = 1.0;
  return static_cast<Image_t>(h);
}

template <class T, class Image_t>
auto Camera<T, Image_t>::render(const HittableList<T>& world) const noexcept
    -> void {
  auto cout_color = write_color(std::cout);
  auto lray_color = [this, &world](auto&& ray) {
    return ray_color(ray, m_max_depth, world);
  };
  auto generate_color = [samples = m_samples_per_pixel,
                         scale = m_pixel_samples_scale, make_ray = get_ray(),
                         &lray_color](auto&& pair) {
    auto lmake_ray = [&make_ray, &pair](auto) { return make_ray(pair); };
    if (pair.first == 0 && pair.second % 5 == 0) {
      std::clog << "Row: " << pair.second << "\n";
    }
    const auto pipe = std::views::iota(0u, samples) |
                      std::views::transform(lmake_ray) |
                      std::views::transform(lray_color);
    const auto c =
        std::ranges::fold_left(pipe, Color<T>{0, 0, 0}, std::plus<>());
    return c * scale;
  };

  const auto rows = std::views::iota(0u, m_img_height);
  const auto cols = std::views::iota(0u, m_img_width);

  std::clog << "===   START   ===\nnumber of rows: " << m_img_height << "\n"
            << std::flush;
  const auto start_time = std::chrono::high_resolution_clock::now();

  std::cout << "P3\n" << m_img_width << ' ' << m_img_height << "\n255\n";
  std::ranges::for_each(utiltools::cartesian_prod(rows, cols) |
                            std::views::transform(generate_color),
                        cout_color);

  std::clog << "===   DONE    ===\n";
  const auto end_time = std::chrono::high_resolution_clock::now();
  const auto duration =
      std::chrono::duration<float, std::chrono::minutes::period>(end_time -
                                                                 start_time);
  std::clog << "took: " << duration.count() << " min\n";
}

template <class T, class Image_t>
auto Camera<T, Image_t>::ray_color(const Ray<T>& ray,
                                   const int depth,
                                   const HittableList<T>& world) const noexcept
    -> Color<T> {
  if (depth <= 0)
    return Color<T>{0., 0., 0.};

  const auto inf_interval = Interval{0.001, globals::infinity<T>};
  if (const auto hit_record = world.hit(ray, inf_interval)) {
    if (const auto scattered = std::visit(
            material_scatter(ray, hit_record.value()), hit_record->mat)) {
      const auto [s_ray, attenuation] = scattered.value();
      return attenuation * ray_color(s_ray, depth - 1, world);
    }
    return Color<T>{0., 0., 0.};
  }

  return backgound_color(ray.direction());
}

template <class T, class Image_t>
auto Camera<T, Image_t>::backgound_color(
    const Vec3<T>& direction) const noexcept -> const Color<T> {
  const auto unit_direction = unit_vector<T>(direction);
  const auto a = 0.5 * (unit_direction.y() + 1.);
  const auto c = (1. - a) * Color<T>{1., 1., 1.} + a * Color<T>{0.5, 0.7, 1.0};
  return c;
}

template <class T, class Image_t>
auto Camera<T, Image_t>::get_ray() const noexcept {
  return [this](auto pair) {
    const auto i = static_cast<T>(pair.first);
    const auto j = static_cast<T>(pair.second);
    const auto offset = sample_square();
    const auto pixel_sample = m_viewport.pixel00_loc() +
                              ((i + offset.x()) * m_viewport.pixel_du()) +
                              ((j + offset.y()) * m_viewport.pixel_dv());
    const auto ray_origin = (m_viewport.defocus_angle() <= 0)
                                ? m_viewport.camera_center()
                                : defocus_disk_sample();
    const auto ray_direction = pixel_sample - ray_origin;
    return Ray<T>{ray_origin, ray_direction};
  };
}

template <class T, class Image_t>
auto Camera<T, Image_t>::sample_square() const noexcept -> Vec3<T> {
  return Vec3<T>(globals::random_t<T>() - 0.5, globals::random_t<T>() - 0.5, 0);
}

template <class T, class Image_t>
auto Camera<T, Image_t>::defocus_disk_sample() const noexcept
    -> const Point3<T> {
  const auto p = Vec3<T>::random_in_unit_disk();
  return m_viewport.camera_center() + (p.x() * m_viewport.defocus_disk_u()) +
         (p.y() * m_viewport.defocus_disk_v());
}

template <class T, class Image_t>
auto Camera<T, Image_t>::material_scatter(
    const Ray<T>& ray,
    const HitRecord<T>& hit_record) const noexcept {
  return overloaded{
      [](std::monostate) -> std::optional<ScatterData_t<T>> {
        return std::nullopt;
      },
      [&ray, &hit_record](const auto& m) -> std::optional<ScatterData_t<T>> {
        return m.scatter(ray, hit_record);
      },
  };
}

#endif  // !CAMERA_HPP
