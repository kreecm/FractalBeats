#include "fractal2d.h"

class mandelbrot : public fractal2d {
 public:
  mandelbrot(int resolution, int max_iter);

 protected:
  void generate() final;

 private:
  const int m_resolution;
  const int m_max_iter;

  bool test_for_escape(const point& p);
};
