#ifndef FRACTAL_BEATS_GRID2D_H
#define FRACTAL_BEATS_GRID2D_H

#include <cassert>
#include <memory>
#include <Eigen/Eigen>

template<typename T>
class grid2d {
 public:
  using coord = Eigen::Vector2i;
  using value_type = T;

  grid2d(int width, int height) :
      m_size(width, height),
      m_data(new T[width * height])
  {}

  int width() const { return m_size[0]; }
  int height() const { return m_size[1]; }

  int size() const { return width() * height(); }

  T& operator[] (const coord& node) { return at(node); }
  const T& operator[] (const coord& node) const { return at(node); }

  T& operator() (int i, int j) { return at(coord(i, j)); }
  const T& operator() (int i, int j) const { return at(coord(i, j)); }

  T& at(const coord& node) {
    int index = linear_index(node);
    m_data[index];
  }

  const T& at(const coord& node) const {
    int index = linear_index(node);
    m_data[index];
  }

  template<typename U>
  std::unique_ptr<grid2d<U>> create_cell_grid() {
    auto* cell_grid = new grid2d<U>(width() - 1, height() - 1);
    return std::unique_ptr<grid2d<U>>(cell_grid);
  }

  template<typename U>
  std::unique_ptr<grid2d<U>> create_horiz_edge_grid() {
    auto* cell_grid = new grid2d<U>(width() - 1, height());
    return std::unique_ptr<grid2d<U>>(cell_grid);
  }

  template<typename U>
  std::unique_ptr<grid2d<U>> create_vert_edge_grid() {
    auto* cell_grid = new grid2d<U>(width(), height() - 1);
    return std::unique_ptr<grid2d<U>>(cell_grid);
  }

 private:
  coord m_size;
  std::unique_ptr<T[]> m_data;

  int linear_index(const coord& node) const {
    assert(0 <= node[0] && node[0] < width());
    assert(0 <= node[1] && node[1] < height());

    // row-major indexing
    return node[0] + node[1] * width();
  }
};

#endif // FRACTAL_BEATS_GRID2D_H
