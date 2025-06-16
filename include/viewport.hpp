#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <cmath>
#include "globals.hpp"
#include "vec3.hpp"
template <class T>
class Viewport {
 public:
  Viewport() = delete;
  template <class Image_t>
  Viewport(const Image_t& img_width,
           const Image_t& img_height,
           const T& height = 2.,
           const Vec3<T>& lookfrom = {0, 0, 0},
           const Vec3<T>& lookat = {0, 0, -1},
           const Vec3<T>& v_up = {0, 1, 0},
           const T& defocus_angle = 0,
           const T& focus_dist = 10.)
      : m_height(height),
        m_width(get_viewport_width(height, img_width, img_height)),

        m_camera_center(lookfrom),
        m_defocus_angle(defocus_angle),
        m_focus_dist(focus_dist),

        m_w(unit_vector(lookfrom - lookat)),
        m_u(unit_vector(cross(v_up, m_w))),
        m_v(cross(m_w, m_u)),

        m_viewport_u(m_width * m_u),
        m_viewport_v(m_height * (-m_v)),
        m_pixel_du(calculate_pixel_delta(img_width, m_viewport_u)),
        m_pixel_dv(calculate_pixel_delta(img_height, m_viewport_v)),
        m_upper_left(m_camera_center - (focus_dist * m_w) -
                     (m_viewport_u + m_viewport_v) / 2.),
        m_pixel00_loc(m_upper_left + 0.5 * (m_pixel_du + m_pixel_dv)),
        m_defocus_disk_u(m_u *
                         get_defocus_radius(m_focus_dist, m_defocus_angle)),
        m_defocus_disk_v(m_v *
                         get_defocus_radius(m_focus_dist, m_defocus_angle)) {}

  [[nodiscard]] auto camera_center() const noexcept -> const Vec3<T> {
    return m_camera_center;
  }
  [[nodiscard]] auto u() const noexcept -> const Vec3<T> {
    return m_viewport_u;
  };
  [[nodiscard]] auto v() const noexcept -> const Vec3<T> {
    return m_viewport_v;
  }
  [[nodiscard]] auto pixel_du() const noexcept -> const Vec3<T> {
    return m_pixel_du;
  }
  [[nodiscard]] auto pixel_dv() const noexcept -> const Vec3<T> {
    return m_pixel_dv;
  }
  [[nodiscard]] auto upper_left() const noexcept -> const Vec3<T> {
    return m_upper_left;
  }
  [[nodiscard]] auto pixel00_loc() const noexcept -> const Vec3<T> {
    return m_pixel00_loc;
  };
  [[nodiscard]] auto defocus_angle() const noexcept -> const T {
    return m_defocus_angle;
  };
  [[nodiscard]] auto defocus_disk_u() const noexcept -> const Vec3<T> {
    return m_defocus_disk_u;
  };
  [[nodiscard]] auto defocus_disk_v() const noexcept -> const Vec3<T> {
    return m_defocus_disk_v;
  };

 private:
  template <class Image_t>
  [[nodiscard]] constexpr auto get_viewport_width(const T& height,
                                                  const Image_t& img_width,
                                                  const Image_t& img_height)
      -> T;
  template <class Image_t>
  [[nodiscard]] auto calculate_pixel_delta(const Image_t& range,
                                           const Vec3<T>& p) const noexcept
      -> const Vec3<T>;

  [[nodiscard]] auto get_defocus_radius(const T& focus_dist,
                                        const T& defocus_angle) const noexcept
      -> const T;

  const T m_height{};
  const T m_width{};

  const Vec3<T> m_camera_center{};
  const T m_defocus_angle{};
  const T m_focus_dist{};

  const Vec3<T> m_w{};
  const Vec3<T> m_u{};
  const Vec3<T> m_v{};
  const Vec3<T> m_viewport_u{};
  const Vec3<T> m_viewport_v{};
  const Vec3<T> m_pixel_du{};
  const Vec3<T> m_pixel_dv{};
  const Vec3<T> m_upper_left{};
  const Vec3<T> m_pixel00_loc{};

  const Vec3<T> m_defocus_disk_u{};
  const Vec3<T> m_defocus_disk_v{};
};

template <class T>
template <class Image_t>
constexpr auto Viewport<T>::get_viewport_width(const T& height,
                                               const Image_t& img_width,
                                               const Image_t& img_height) -> T {
  return height * static_cast<T>(img_width) / static_cast<T>(img_height);
}

template <class T>
template <class Image_t>
auto Viewport<T>::calculate_pixel_delta(const Image_t& range,
                                        const Vec3<T>& p) const noexcept
    -> const Vec3<T> {
  return p / static_cast<T>(range);
}

template <class T>
auto Viewport<T>::get_defocus_radius(const T& focus_dist,
                                     const T& defocus_angle) const noexcept
    -> const T {
  return focus_dist * std::tan(globals::degrees_to_radians(defocus_angle / 2.));
}

#endif  // !VIEWPORT_HPP
