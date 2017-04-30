#ifndef FRACTAL_BEATS_FRACTAL2D_H
#define FRACTAL_BEATS_FRACTAL2D_H

#include <ostream>
#include <Eigen/Eigen>
#include "grid2d.h"

class fractal2d {
 public:
  using point = Eigen::Vector2f;
  using grid  = grid2d<bool>;
  using coord = grid::coord;

  fractal2d(int width, int height, point min, point max);

  virtual void generate() =0;

  int width() const { return m_grid.width(); }
  int height() const { return m_grid.height(); }

  const point& min() const { return m_min; }
  const point& max() const { return m_max; }

  grid& get_grid() { return m_grid; }
  const grid& get_grid() const { return m_grid; }

  void print(std::ostream& out) const;

  bool operator() (int i, int j) const { return m_grid.at(coord(i, j)); }

 protected:
  void add_grid_point_to_set(int i, int j);
  point get_grid_point(int i, int j) const;

 private:
  const point m_min;
  const point m_max;
  const float m_dx;
  const float m_dy;

  grid m_grid;
};

#endif // FRACTAL_BEATS_FRACTAL2D_H
