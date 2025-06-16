#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <ranges>
#include <tuple>
#include <variant>
#include "color.hpp"
#include "fn_cpp_helper.hpp"
#include "globals.hpp"
#include "hit_record.hpp"
#include "hittable_list.hpp"
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
                     const Vec3<T>& v_up)
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
                               m_focal_lenght,
                               2 * m_h * m_focal_lenght,
                               lookfrom,
                               lookat,
                               v_up)),
        m_max_depth(max_depth) {}

  auto render(const HittableList<T>& world) const noexcept -> void;

 private:
  template <class Ratio>
  constexpr auto get_height(const Image_t& width, const Ratio& ratio)
      -> const Image_t;

  [[nodiscard]] auto ray_color(Ray<T>& ray,
                               const int depth,
                               const HittableList<T>& world) const noexcept
      -> Color<T>;
  [[nodiscard]] auto get_ray() const noexcept;
  [[nodiscard]] auto sample_square() const noexcept -> Vec3<T>;
  [[nodiscard]] auto material_scatter(Ray<T>& ray,
                                      HitRecord<T>& hit_record) const noexcept;

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
  auto lray_color = [this, &world](auto ray) {
    return ray_color(ray, m_max_depth, world);
  };
  auto generate_color = [samples = m_samples_per_pixel,
                         scale = m_pixel_samples_scale, make_ray = get_ray(),
                         &lray_color](auto pair) {
    auto lmake_ray = [&make_ray, &pair](auto) { return make_ray(pair); };
    const auto pipe = std::views::iota(0u, samples) |
                      std::views::transform(lmake_ray) |
                      std::views::transform(lray_color);
    const auto c =
        std::ranges::fold_left(pipe, Color<T>{0, 0, 0}, std::plus<>());
    return c * scale;
  };

  const auto rows = std::views::iota(0u, m_img_height);
  const auto cols = std::views::iota(0u, m_img_width);

  std::clog << "===   START   ===\n" << std::flush;
  const auto start_time = std::chrono::high_resolution_clock::now();

  std::cout << "P3\n" << m_img_width << ' ' << m_img_height << "\n255\n";
  std::ranges::for_each(utiltools::cartesian_prod(rows, cols) |
                            std::views::transform(generate_color),
                        cout_color);

  std::clog << "===   DONE    ===\n";
  const auto end_time = std::chrono::high_resolution_clock::now();
  const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);
  std::clog << "took: " << duration.count() << "ms\n";
}

template <class T, class Image_t>
auto Camera<T, Image_t>::ray_color(Ray<T>& ray,
                                   const int depth,
                                   const HittableList<T>& world) const noexcept
    -> Color<T> {
  if (depth <= 0)
    return Color<T>{0., 0., 0.};

  HitRecord<T> hit_record{};

  if (world.hit(ray, Interval{0.001, globals::infinity<T>}, hit_record)) {
    if (auto [was_scattered, s_ray, attenuation] =
            std::visit(material_scatter(ray, hit_record), hit_record.mat);
        was_scattered == true) {
      return attenuation * ray_color(s_ray, depth - 1, world);
    }
    return Color<T>{0., 0., 0.};
  }

  const auto unit_direction = unit_vector<T>(ray.direction());
  const auto a = 0.5 * (unit_direction.y() + 1.);
  const auto c = (1. - a) * Color<T>{1., 1., 1.} + a * Color<T>{0.5, 0.7, 1.0};
  return c;
}

template <class T, class Image_t>
auto Camera<T, Image_t>::get_ray() const noexcept {
  return [this](auto pair) {
    const auto i = static_cast<T>(pair.first);
    const auto j = static_cast<T>(pair.second);
    auto offset = sample_square();
    auto pixel_sample = m_viewport.pixel00_loc() +
                        ((i + offset.x()) * m_viewport.pixel_du()) +
                        ((j + offset.y()) * m_viewport.pixel_dv());
    auto ray_origin = m_viewport.camera_center();
    auto ray_direction = pixel_sample - ray_origin;
    return Ray<T>{ray_origin, ray_direction};
  };
}

template <class T, class Image_t>
auto Camera<T, Image_t>::sample_square() const noexcept -> Vec3<T> {
  return Vec3<T>(globals::random_t<T>() - 0.5, globals::random_t<T>() - 0.5, 0);
}

template <class T, class Image_t>
auto Camera<T, Image_t>::material_scatter(Ray<T>& ray, HitRecord<T>& hit_record)
    const noexcept {
  return overloaded{
      [](std::monostate) {
        return std::make_tuple(false, Ray<T>{}, Color<T>{});
      },
      [&ray, &hit_record](const auto& m) {
        auto scattered_ray = Ray<T>{};
        auto attenuation = Color<T>{};
        auto was_scattered =
            m.scatter(ray, hit_record, attenuation, scattered_ray);
        return std::make_tuple(was_scattered, scattered_ray, attenuation);
      },
  };
}

#endif  // !CAMERA_HPP
