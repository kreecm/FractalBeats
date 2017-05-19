#include <complex>

#include "mandelbrot.h"

// Default domain is [-2.5, 0.5] x [-1, 1].
// The resolution specifies the number of grid points in the y-direction.
// The number of grid points in the x-direction is 1.5 * resolution
// (since the domain has 3:2 aspect ratio).

mandelbrot::mandelbrot(int resolution, int max_iter)
    : fractal2d(point(-2.5, 1.0),
                point(0.5, -1.0),
                1.5 * resolution,
                resolution),
      m_resolution(resolution),
      m_max_iter(max_iter)
{}

bool mandelbrot::test_point(const point& p) {
  const std::complex<double> z0(p[0], p[1]);
  std::complex<double> z(0, 0);
  for (int iter = 0; iter < m_max_iter; iter++) {
    if (std::abs(z) > 4.0) {    // escape occurs if the absolute value of z ever exceeds 2
      return false;             // this means that it is definitely not in the Mandelbrot set
    }
    z = z * z + z0;             // this is the dynamic process defining the Mandelbrot set
  }                             // if it has not escaped after m_max_iter iterations, then we assert that it never escapes
  return true;                  // in this case, we assert that it is in the Mandelbrot set
}

void mandelbrot::generate() {
  for (int j = 0; j < height(); j++) {
    for (int i = 0; i < width(); i++) {
      point p = get_grid_point(i, j);
      if (test_point(p)) {
        add_grid_point_to_set(i, j);    // we change the relevant values of the indicator function
      }
    }
  }
}
