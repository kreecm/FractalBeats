#include <ostream>
#include <vector>

#include <Eigen/Eigen>

class fractal2d {
 public:
  using point = Eigen::Vector2f;
  using size = Eigen::Vector2i;

  fractal2d(point min, point max, size resolution);

  virtual void generate() =0;

  int width() const { return m_resolution[0]; }
  int height() const { return m_resolution[1]; }

  const point& min() const { return m_min; }
  const point& max() const { return m_max; }

  void print(std::ostream& out) const;

  bool operator() (int i, int j) const { return get_grid_point_in_set(i, j); }

 protected:
  int index_of_grid_point(int i, int j) const;
  void add_grid_point_to_set(int i, int j);
  bool get_grid_point_in_set(int i, int j) const;
  point get_grid_point(int i, int j) const;

 private:
  const point m_min;
  const point m_max;
  const size m_resolution;
  const float m_dx;
  const float m_dy;

  std::vector<bool> m_pointset;
};
