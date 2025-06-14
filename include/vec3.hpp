#ifndef VEC3_HPP
#define VEC3_HPP

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <format>
#include <iostream>
#include <ostream>
#include <type_traits>
#include "fn_cpp_helper.hpp"
#include "globals.hpp"

#define LOG_V(v) std::clog << (v) << "\n"

template <typename T,
          typename = typename std::enable_if_t<std::is_arithmetic_v<T>, T>>
class Vec3 {
 public:
  Vec3() : m_x(), m_y(), m_z() {};
  Vec3(const T& x, const T& y, const T& z) : m_x(x), m_y(y), m_z(z) {};

  using type = T;

  [[nodiscard]] auto x() const noexcept -> const T& { return m_x; };
  [[nodiscard]] auto y() const noexcept -> const T& { return m_y; };
  [[nodiscard]] auto z() const noexcept -> const T& { return m_z; };

  [[nodiscard]] auto length_squared() const noexcept -> T {
    return m_x * m_x + m_y * m_y + m_z * m_z;
  }

  [[nodiscard]] auto length() const noexcept -> T {
    return std::sqrt(length_squared());
  }

  [[nodiscard]] auto operator-() const noexcept -> Vec3<T> {
    return Vec3(-m_x, -m_y, -m_z);
  };

  auto operator+=(const Vec3& other) noexcept -> Vec3<T>& {
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
    return *this;
  }

  auto operator*=(const T& t) noexcept -> Vec3<T>& {
    m_x *= t;
    m_y *= t;
    m_z *= t;
    return *this;
  }

  auto operator/=(const T& t) noexcept -> Vec3<T>& { return *this *= 1 / t; }

  template <class Fn>
  auto apply_element_wise(Fn&& f) noexcept -> void {
    using pipeline::operator|;
    m_x = m_x | f;
    m_y = m_y | f;
    m_z = m_z | f;
  }

  [[nodiscard]] static auto random(const T& min, const T& max) noexcept
      -> Vec3<T> {
    return Vec3<T>(globals::random_t<T>(min, max),
                   globals::random_t<T>(min, max),
                   globals::random_t<T>(min, max));
  }

  [[nodiscard]] static auto random() noexcept -> Vec3<T> {
    return random(0, 1);
  }

  // TODO: different than in book cuz i didn't like while(true)
  [[nodiscard]] static inline auto random_in_unit_sphere() noexcept -> Vec3<T> {
    // using pipeline::operator|;
    // auto is_in = [](const auto& v) { return v.length_squared() < 1; };
    // auto norm = [&is_in](auto v) {
    //   return (is_in(v)) ? v : v /
    //   globals::random_t<T>(v.length_squared(), 2.);
    // };
    // return Vec3<T>::random(-1, 1) | norm;
    while (true) {
      auto p = Vec3<T>::random(-1., 1.);
      if (p.length_squared() < 1)
        return p;
    }
  }

  [[nodiscard]] static inline auto random_unit_vector() -> Vec3<T> {
    return unit_vector<T>(Vec3<T>::random_in_unit_sphere());
  }

  [[nodiscard]] static inline auto random_on_hemisphere(
      const Vec3<T>& normal) noexcept -> Vec3<T> {
    const auto on_unit_sphere = random_unit_vector();
    return (dot(on_unit_sphere, normal) > 0.) ? on_unit_sphere
                                              : -on_unit_sphere;
  }

  [[nodiscard]] auto near_zero() const noexcept -> bool {
    using pipeline::operator|;
    auto less_than_eps = [](auto v) { return v < 1e-8; };
    auto labs = [](auto v) { return std::abs(v); };
    const auto fx = m_x | labs | less_than_eps;
    const auto fy = m_y | labs | less_than_eps;
    const auto fz = m_z | labs | less_than_eps;
    return fx && fy && fz;
  }

 private:
  T m_x{}, m_y{}, m_z{};
};

template <class T>
[[nodiscard]] inline auto operator<<(std::ostream& out,
                                     const Vec3<T>& v) noexcept
    -> std::ostream& {
  return out << std::format("{} {} {}", v.x(), v.y(), v.z());
}

template <class T>
[[nodiscard]] inline auto operator+(const Vec3<T>& u, const Vec3<T>& v) noexcept
    -> Vec3<T> {
  return Vec3<T>(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

template <class T>
[[nodiscard]] inline auto operator-(const Vec3<T>& u, const Vec3<T>& v) noexcept
    -> Vec3<T> {
  auto x = u.x() - v.x();
  auto y = u.y() - v.y();
  auto z = u.z() - v.z();
  return Vec3<T>(x, y, z);
}

template <class T>
[[nodiscard]] inline auto operator-(const Vec3<T>& v) noexcept -> Vec3<T> {
  return Vec3<T>(-v.x(), -v.y(), -v.z());
}

template <class T>
[[nodiscard]] inline auto operator*(const Vec3<T>& u, const Vec3<T>& v) noexcept
    -> Vec3<T> {
  return Vec3<T>(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

template <class T>
[[nodiscard]] inline auto operator*(const Vec3<T>& v, const T& t) noexcept
    -> Vec3<T> {
  auto u{v};
  return u *= t;
}

template <class T>
[[nodiscard]] inline auto operator*(const T& t, const Vec3<T>& v) noexcept
    -> Vec3<T> {
  return v * t;
}

template <class T>
[[nodiscard]] inline auto operator/(const Vec3<T>& v, const T& t) noexcept
    -> Vec3<T> {
  auto u{v};
  u /= t;
  return u;
}

template <class T>
[[nodiscard]] inline auto dot(const Vec3<T>& u, const Vec3<T>& v) noexcept
    -> T {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

template <class T>
[[nodiscard]] inline auto cross(const Vec3<T>& u, const Vec3<T>& v) noexcept
    -> Vec3<T> {
  return Vec3<T>(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
                 u.x() * v.y() - u.y() * v.x());
}

template <class T>
[[nodiscard]] inline auto unit_vector(const Vec3<T>& v) noexcept -> Vec3<T> {
  return v / v.length();
}

template <class T>
[[nodiscard]] inline auto reflect(const Vec3<T>& v, const Vec3<T>& n) noexcept
    -> Vec3<T> {
  return v - 2 * dot(v, n) * n;
}

template <class T>
[[nodiscard]] inline auto refract(const Vec3<T>& uv,
                                  const Vec3<T>& n,
                                  const T& etai_over_etat) noexcept -> Vec3<T> {
  const auto cos_theta = std::min<T>(dot(-uv, n), 1.);
  const auto r_out_perp = etai_over_etat * (uv + cos_theta * n);
  const auto r_out_parallel =
      -std::sqrt(std::abs(1. - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}

#endif  // !VEC3_HPP
