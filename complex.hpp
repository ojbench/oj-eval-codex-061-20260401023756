#ifndef SJTU_COMPLEX_HPP
#define SJTU_COMPLEX_HPP

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>

// You must not include any other headers.

namespace sjtu {

class divided_by_zero final : public std::exception {
public:
  divided_by_zero() = default;
  ~divided_by_zero() = default;
  const char *what() const noexcept override { return "complex divided by zero!"; }
};

inline int sign(double x) {
  const double eps = 1e-6;
  return (x > eps) - (x < -eps);
}

class complex {
private:
  double a, b; // a + bi

public:
  // Constructors
  complex() : a(0.0), b(0.0) {}
  complex(double ra) : a(ra), b(0.0) {}
  complex(double ra, double ib) : a(ra), b(ib) {}

  // Accessors (references)
  double &real() { return a; }
  double &imag() { return b; }

  // Unary operators
  complex operator-() const { return complex(-a, -b); }
  complex operator~() const { return complex(a, -b); }

  // Binary arithmetic
  complex operator+(const complex &rhs) const { return complex(a + rhs.a, b + rhs.b); }
  complex operator-(const complex &rhs) const { return complex(a - rhs.a, b - rhs.b); }
  complex operator*(const complex &rhs) const {
    return complex(a * rhs.a - b * rhs.b, a * rhs.b + b * rhs.a);
  }
  complex operator/(const complex &rhs) const {
    double denom = rhs.a * rhs.a + rhs.b * rhs.b;
    if (sign(rhs.a) == 0 && sign(rhs.b) == 0) {
      throw divided_by_zero();
    }
    return complex((a * rhs.a + b * rhs.b) / denom, (b * rhs.a - a * rhs.b) / denom);
  }

  // Compound assignments
  complex &operator+=(const complex &rhs) {
    a += rhs.a;
    b += rhs.b;
    return *this;
  }
  complex &operator-=(const complex &rhs) {
    a -= rhs.a;
    b -= rhs.b;
    return *this;
  }
  complex &operator*=(const complex &rhs) {
    double ra = a * rhs.a - b * rhs.b;
    double ib = a * rhs.b + b * rhs.a;
    a = ra;
    b = ib;
    return *this;
  }
  complex &operator/=(const complex &rhs) {
    if (sign(rhs.a) == 0 && sign(rhs.b) == 0) {
      throw divided_by_zero();
    }
    double denom = rhs.a * rhs.a + rhs.b * rhs.b;
    double ra = (a * rhs.a + b * rhs.b) / denom;
    double ib = (b * rhs.a - a * rhs.b) / denom;
    a = ra;
    b = ib;
    return *this;
  }

  // Comparison
  bool operator==(const complex &rhs) const {
    return sign(a - rhs.a) == 0 && sign(b - rhs.b) == 0;
  }

  // Conversion to bool
  explicit operator bool() const { return !(sign(a) == 0 && sign(b) == 0); }

  // Output
  friend std::ostream &operator<<(std::ostream &os, const complex &x) {
    // Prepare values with epsilon handling
    double r = (sign(x.a) == 0) ? 0.0 : x.a;
    double i = (sign(x.b) == 0) ? 0.0 : x.b;

    os.setf(std::ios::fixed);
    os << std::setprecision(6);

    // Real part: only print '-' if strictly negative beyond eps
    if (sign(r) < 0) {
      os << "-" << std::fabs(r);
    } else {
      os << std::fabs(r);
    }

    // Imag sign: '-' if negative beyond eps, else '+'
    os << (sign(i) < 0 ? '-' : '+');
    os << std::fabs(i) << 'i';
    return os;
  }
};

} // namespace sjtu

#endif // SJTU_COMPLEX_HPP

