#ifndef FRACTAL_BEATS_MANDELBROT_H
#define FRACTAL_BEATS_MANDELBROT_H

#include "fractal2d.h"

class mandelbrot : public fractal2d {
    
    // Default domain is [-2.5, 0.5] x [-1, 1].
    // The resolution specifies the number of grid points in the y-direction.
    // The number of grid points in the x-direction is 1.5 * resolution
    // (since the domain has 3:2 aspect ratio).
    
    private:
        const int m_resolution;
        const int m_max_iter;               // this is the threshold after which we assert escape will not occur
    
    public:
        mandelbrot(int resolution, int max_iter);

    private:
        bool test_point(const point& p);    // this tests an arbitrary complex number to see whether it is in M
    
    protected:
        void generate() final;              // this generates the Mandelbrot set
};

#endif // FRACTAL_BEATS_MANDELBROT_H
