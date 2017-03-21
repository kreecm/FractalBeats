#include <complex>

#include "mandelbrot.h"

mandelbrot::mandelbrot(int resolution, int max_iter)
    : fractal2d(point(-2.0 - 1.0 / resolution, 1.0 + 1.0 / resolution),
                point(1.0 / resolution, -2.0 - 1.0 / resolution),
                size(2 * resolution + 3, 3 * resolution + 3)),
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
