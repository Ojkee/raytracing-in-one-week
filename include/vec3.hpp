#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <format>
#include <ostream>
#include <type_traits>

template <typename T,
          typename = typename std::enable_if_t<std::is_arithmetic_v<T>, T>>
class Vec3 {
 public:
  Vec3() : m_x(), m_y(), m_z() {};
  Vec3(const T& x, const T& y, const T& z) : m_x(x), m_y(y), m_z(z) {};

  Vec3(const Vec3& other) = default;
  Vec3(Vec3&& other) = default;

  using type = T;

  [[nodiscard]] auto x() const noexcept -> const T& { return m_x; };
  [[nodiscard]] auto y() const noexcept -> const T& { return m_y; };
  [[nodiscard]] auto z() const noexcept -> const T& { return m_z; };

  [[nodiscard]] auto operator-() noexcept -> Vec3<T> {
    return Vec3(-m_x, -m_y, -m_z);
  };

  Vec3<T>& operator+=(const Vec3& other) noexcept {
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;
  }

  Vec3<T>& operator*=(const T& t) noexcept {
    m_x *= t;
    m_y *= t;
    m_z *= t;
    return *this;
  }

  Vec3<T>& operator/=(const T& t) noexcept { return *this *= 1 / t; }

  template <class R>
  [[nodiscard]] R length_squared() const noexcept {
    return static_cast<R>(m_x * m_x + m_y * m_y + m_z * m_z);
  }

  [[nodiscard]] auto length() const noexcept -> T {
    return std::sqrt(length_squared<T>());
  }

 private:
  T m_x{}, m_y{}, m_z{};
};

template <class T>
inline auto operator<<(std::ostream& out, const Vec3<T>& v) -> std::ostream& {
  return out << std::format("{} {} {}", v.x(), v.y(), v.z());
}

template <class T>
inline auto operator+(const Vec3<T>& u, const Vec3<T>& v) -> Vec3<T> {
  return Vec3<T>(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

template <class T>
inline auto operator-(const Vec3<T>& u, const Vec3<T>& v) -> Vec3<T> {
  auto x = u.x() - v.x();
  auto y = u.y() - v.y();
  auto z = u.z() - v.z();
  return Vec3<T>(x, y, z);
}

template <class T>
inline auto operator*(const Vec3<T>& u, const Vec3<T>& v) -> Vec3<T> {
  return Vec3<T>(u.x() * v.x(), u.y() * v.y(), u.z() * u.z());
}

template <class T>
inline auto operator*(const Vec3<T>& v, const T& t) -> Vec3<T> {
  auto u{v};
  return u *= t;
}

template <class T>
inline auto operator*(const T& t, const Vec3<T>& v) -> Vec3<T> {
  return v * t;
}

template <class T>
inline auto operator/(const Vec3<T>& v, const T& t) -> Vec3<T> {
  auto u{v};
  u /= t;
  return u;
}

template <class T>
inline auto dot(const Vec3<T>& u, const Vec3<T>& v) -> T {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

template <class T>
inline auto cross(const Vec3<T>& u, const Vec3<T>& v) -> Vec3<T> {
  return Vec3<T>(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
                 u.x() * v.y() - u.y() * v.x());
}

template <class R, class T>
inline auto unit_vector(const Vec3<T>& v) -> Vec3<R> {
  return v / v.length();
}

#endif  // !VEC3_HPP
