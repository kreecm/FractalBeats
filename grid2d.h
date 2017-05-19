#ifndef FRACTAL_BEATS_GRID2D_H
#define FRACTAL_BEATS_GRID2D_H

#include <cassert>
#include <memory>
#include <Eigen/Eigen>

template<typename T>
class grid2d {
    public:
        using value_type = T;
        using coord = Eigen::Vector2i;                  // for pairs of integers

    
    private:
        std::unique_ptr<T[]> m_data;                    // we store a grid's worth of values of type T
        coord m_size;                                   // the size of the grid is given by a pair of integers
    
    public:
        grid2d(int width, int height) :                 // we can initialize a grid of certain width and height
            m_data(new T[width * height]()),
            m_size(width, height)
        {}

        int width() const { return m_size[0]; }
        int height() const { return m_size[1]; }
        int size() const { return width() * height(); }

    
    private:
        int linear_index(const coord& node) const {     // since we store a grid's worth of data in an array
            assert(0 <= node[0] && node[0] < width());
            assert(0 <= node[1] && node[1] < height());
        
            return node[0] + node[1] * width();         // we use row-major indexing
        }
    
    private:
        T& at(const coord& node) {                      // we can look at an element in the grid
            int index = linear_index(node);
            return m_data[index];
        }
    
        const T& at(const coord& node) const {          // even if the grid is constant
            int index = linear_index(node);
            return m_data[index];
        }

    public:
        T& operator[] (const coord& node) { return at(node); }                              // public notation is
        const T& operator[] (const coord& node) const { return at(node); }                  // [] if we use a coord

        T& operator() (const int i, const int j) { return at(coord(i, j)); }                // public notation is ()
        const T& operator() (const int i, const int j) const { return at(coord(i, j)); }    // if we use a pair of ints

    /**
     * Constructs a new grid that is smaller in each dimension by one.
     *
     * If we consider the calling object to store its data at the nodes of a
     * 2-dimensional grid, the returned object is suitable for storing data at
     * the cells of the grid.
     */
        template<typename U>
        std::unique_ptr<grid2d<U>> create_cell_grid() {
            auto* cell_grid = new grid2d<U>(width() - 1, height() - 1);
            return std::unique_ptr<grid2d<U>>(cell_grid);
        }
    /**
     * Constructs a new grid that is smaller in horizontal dimension by one.
     *
     * If we consider the calling object to store its data at the nodes of a
     * 2-dimensional grid, the returned object is suitable for storing data on
     * the horizontal edges of the grid.
     */
        template<typename U>
        std::unique_ptr<grid2d<U>> create_horiz_edge_grid() {
            auto* cell_grid = new grid2d<U>(width() - 1, height());
            return std::unique_ptr<grid2d<U>>(cell_grid);
        }
    /**
     * Constructs a new grid that is smaller in vertical dimension by one.
     *
     * If we consider the calling object to store its data at the nodes of a
     * 2-dimensional grid, the returned object is suitable for storing data on
     * the vertical edges of the grid.
     */
        template<typename U>
        std::unique_ptr<grid2d<U>> create_vert_edge_grid() {
            auto* cell_grid = new grid2d<U>(width(), height() - 1);
            return std::unique_ptr<grid2d<U>>(cell_grid);
        }
};

#endif // FRACTAL_BEATS_GRID2D_H
