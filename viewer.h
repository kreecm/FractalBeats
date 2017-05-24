#ifndef FRACTAL_BEATS_VIEWER_H
#define FRACTAL_BEATS_VIEWER_H

#include <memory>

#include "fractal2d.h"

/**
 * A simple viewer class for rendering a fractal2d.
 *
 * We only allow one viewer to be created, thus all the methods on the class are
 * static.
 */
class viewer {
 public:
  static void create(int* argc, char** argv);
  static void show();

  static void set_fractal(const fractal2d* fractal) {
    s_viewer->m_fractal = fractal;
  }

  static const fractal2d* get_fractal() {
    return s_viewer->m_fractal;
  }

  static const int get_texture() {
    return s_viewer->m_tex_id;
  }

 private:
  static std::unique_ptr<viewer> s_viewer;

  viewer() : m_fractal(nullptr) {}

  void init();

  const fractal2d* m_fractal;
  int m_tex_id;
};

#endif // FRACTAL_BEATS_VIEWER_H
