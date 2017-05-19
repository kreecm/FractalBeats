#ifndef FRACTAL_BEATS_FRACTAL2D_H
#define FRACTAL_BEATS_FRACTAL2D_H

#include <ostream>
#include <Eigen/Eigen>
#include "grid2d.h"

class fractal2d {

    public:
        using point = Eigen::Vector2f;      // for pairs of floats
        using grid  = grid2d<bool>;         // the grid will store an indicator function, so uses bools
    
    /////// read below for better understanding
    public:
        fractal2d(point min, point max, int width, int height);     // we can initialize the relevant values
        virtual void generate() =0;                                 // we can generate the fractal
        void print(std::ostream& out) const;                        // we can print the fractal

    private:                // we'll look at the fractal intersected with [ m_min[0], m_max[0] ] x [ m_max[1], m_min[1] ]
        const point m_min;  // min is "top-left"
        const point m_max;  // max is "bottom-right"

    public:
        const point& min() const { return m_min; }
        const point& max() const { return m_max; }

    private:
        grid m_grid;        // further, we'll discretize; we'll have m_grid.width() x-values, and m_grid.height() y-values

    public:
        int width() const { return m_grid.width(); }
        int height() const { return m_grid.height(); }
    
    private:
        const float m_dx;   // this corresponds to intersecting with (m_dx Z) x (m_dy Z)
        const float m_dy;
    
        // for 0 <= i < m_grid.width() and 0 <= j < m_grid.height(), m_grid(i,j) is a bool
        // m_grid(i,j) will be the relevant value of the indicator function of the fractal
    
    public:
        bool operator() (int i, int j) { return m_grid(i, j); }                 // allows "fractal(i, j) = true"
        const bool operator() (int i, int j) const { return m_grid(i, j); }     // allows "fractal(i, j) == true"

        // we can basically access all public aspects of m_grid using the public functions of fractal2d
        // so I don't see much use for these functions
    
        // grid& get_grid() { return m_grid; }
        // const grid& get_grid() const { return m_grid; }

    protected:
        void add_grid_point_to_set(int i, int j);   // change value of indicator function to 1
        point get_grid_point(int i, int j) const;   // look up non-discrete point
};

#endif // FRACTAL_BEATS_FRACTAL2D_H
