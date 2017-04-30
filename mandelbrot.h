#ifndef FRACTAL_BEATS_MANDELBROT_H
#define FRACTAL_BEATS_MANDELBROT_H

#include "fractal2d.h"

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
