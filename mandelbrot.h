#ifndef FRACTAL_BEATS_MANDELBROT_H
#define FRACTAL_BEATS_MANDELBROT_H

#include "fractal2d.h"

/**
 * Implements the Mandelbrot set as a fractal2d.
 *
 * The default domain for the Mandelbrot set is [-2.5, 0.5] x [-1, 1]. The
 * resolution specifies the number of grid points in the y-direction. The number
 * of grid points in the x-direction is 1.5 * resolution (since the domain has
 * 3:2 aspect ratio.)
 */
class mandelbrot : public fractal2d {
 public:
  mandelbrot(int resolution, int max_iter);

 protected:
  void generate() final;

 private:
  const int m_resolution;
  const int m_max_iter;

  bool test_point(const point& p);
};

#endif // FRACTAL_BEATS_MANDELBROT_H
