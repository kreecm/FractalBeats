#ifndef FRACTAL_BEATS_FRACTAL2D_H
#define FRACTAL_BEATS_FRACTAL2D_H

#include <ostream>
#include <Eigen/Eigen>
#include "grid2d.h"

/**
 * Base class for defining a 2-dimensional fractal.
 *
 * We treat fractals as subset of R^2, which we store in a grid2d<bool>.  The
 * grid is mapped to a rectangle domain of R^2, which is defined by the min and
 * max points passed to the constructor.
 */
class fractal2d {
 public:
  using point = Eigen::Vector2f;
  using grid_type  = grid2d<bool>;

  fractal2d(int width, int height, point min, point max);

  /**
   * Generate the fractal set.
   */
  virtual void generate() =0;

  int width() const { return m_grid.width(); }
  int height() const { return m_grid.height(); }

  const grid_type& get_grid() const { return m_grid; }
  const point& min() const { return m_min; }
  const point& max() const { return m_max; }

  void print(std::ostream& out) const;

  bool operator() (int i, int j) const { return m_grid(i, j); }

 protected:
  void add_grid_point_to_set(int i, int j);
  point get_grid_point(int i, int j) const;

 private:
  const point m_min;
  const point m_max;
  const float m_dx;
  const float m_dy;

  grid_type m_grid;
};

#endif // FRACTAL_BEATS_FRACTAL2D_H
