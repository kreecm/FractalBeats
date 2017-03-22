#include <complex>

#include "mandelbrot.h"

// Default domain is [-2.5, 0.5] x [-1, 1]. The resolution specifies the number
// of grid points in the y-direction. The number of grid points in the
// x-direction is 1.5 * resolution (since the domain has 3:2 aspect ratio.)
mandelbrot::mandelbrot(int resolution, int max_iter)
    : fractal2d(point(-2.5, 1.0),
                point(0.5, -1.0),
                size(1.5 * resolution, resolution)),
      m_resolution(resolution),
      m_max_iter(max_iter)
{}

bool mandelbrot::test_for_escape(const point& p) {
  std::complex<double> z0(p[0], p[1]);
  std::complex<double> z(0, 0);
  for (int iter = 0; iter < m_max_iter; iter++) {
    if (std::abs(z) >= 4.0) {
      return false;
    }
    z = z * z + z0;
  }
  return true;
}

void mandelbrot::generate() {
  for (int j = 0; j < height(); j++) {
    for (int i = 0; i < width(); i++) {
      point p = get_grid_point(i, j);
      if (test_for_escape(p)) {
        add_grid_point_to_set(i, j);
      }
    }
  }
}
