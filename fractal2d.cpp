#include "fractal2d.h"

fractal2d::fractal2d(int width, int height, point min, point max) :
    m_min(min),
    m_max(max),
    m_dx((m_max[0] - m_min[0]) / width),
    m_dy((m_max[1] - m_min[1]) / height),
    m_grid(width, height)
{}

void fractal2d::print(std::ostream& out) const {
  for (int j = 0; j < height(); j++) {
    for ( int i = 0; i < width(); i++) {
      if (m_grid(i, j)) {
        out << "*";
      }
      else {
        out << " ";
      }
    }
    out << std::endl;
  }
  out << std::endl;
}

void fractal2d::add_grid_point_to_set(int i, int j) {
  m_grid(i, j) = true;
}

fractal2d::point fractal2d::get_grid_point(int i, int j) const {
  return m_min + point(i * m_dx, j * m_dy);
}
