// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Color
// -----------------------------------------------------------------------------

#ifndef BUW_COLOR_HPP
#define BUW_COLOR_HPP

#include <iostream>

struct Color
{

  friend std::ostream& operator<<(std::ostream& os, Color const& c)
  {
    os << "(" << c.r << "," << c.g << "," << c.b << ")\n";
    return os;
  }

  Color& operator+=(Color const& other)
  {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
  }

  Color& operator-=(Color const& other)
  {
    r -= other.r;
    g -= other.g;
    b -= other.b;
    return *this;
  }

  friend Color operator+(Color const& a, Color const& b)
  {
    auto tmp(a);
    tmp += b;
    return tmp;
  }

  friend Color operator-(Color const& a, Color const& b)
  {
    auto tmp(a);
    tmp -= b;
    return tmp;
  }

  Color& operator*=(Color const& other) {
      r *= other.r;
      g *= other.g;
      b *= other.b;
      return *this;
  }

  friend Color operator*(Color const& a, Color const& b) {
      Color tmp = a;
      tmp *= b;
      return tmp;
  }

  Color& operator*=(float koef) {
      r *= koef;
      g *= koef;
      b *= koef;
      return *this;
  }

  friend Color operator*(Color const& a, float koef) {
      Color tmp = a;
      tmp *= koef;
      return tmp;
  }

  float r;
  float g;
  float b;
};



#endif //#define BUW_COLOR_HPP
