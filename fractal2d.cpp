#include "fractal2d.h"

fractal2d::fractal2d(point min, point max, size resolution)
    : m_min(min),
      m_max(max),
      m_resolution(resolution),
      m_dx((m_max[0] - m_min[0]) / m_resolution[0]),
      m_dy((m_max[1] - m_min[1]) / m_resolution[1])
{
  m_pointset.resize(width() * height());
  this->generate();
}

void fractal2d::print(std::ostream& out) const {
  for (int j = 0; j < height(); j++) {
    for( int i = 0; i < width(); i++) {
      if(get_grid_point_in_set(i, j)) {
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

int fractal2d::index_of_grid_point(int i, int j) const {
  if (i < 0 || i >= width() || j < 0 || j >= height()) {
    return -1;
  }
  return i + j * width();
}

void fractal2d::add_grid_point_to_set(int i, int j) {
  int index = index_of_grid_point(i, j);
  m_pointset[index] = true;
}

bool fractal2d::get_grid_point_in_set(int i, int j) const {
  int index = index_of_grid_point(i, j);
  return m_pointset[index];
}

fractal2d::point fractal2d::get_grid_point(int i, int j) const {
  return m_min + point(i * m_dx, j * m_dy);
}
