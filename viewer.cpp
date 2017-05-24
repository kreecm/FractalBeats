#include "viewer.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>

#if defined(__APPLE__) || defined(__MACOSX)
#  include <OpenGL/gl.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glut.h>
#endif

namespace {

void reshape(int w, int h) {
  const fractal2d* fractal = viewer::get_fractal();
  const fractal2d::point& min = fractal->min();
  const fractal2d::point& max = fractal->max();

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(min[0], max[0],
          min[1], max[1],
          -1, 1);
}

void display() {
  const fractal2d* fractal = viewer::get_fractal();
  const int tex_id = viewer::get_texture();
  assert(fractal != nullptr);

  const fractal2d::point& min = fractal->min();
  const fractal2d::point& max = fractal->max();

  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glBegin(GL_TRIANGLE_STRIP);
  {
    glTexCoord2f(0.0, 0.0);
    glVertex2f(min[0], min[1]);

    glTexCoord2f(1.0, 0.0);
    glVertex2f(max[0], min[1]);

    glTexCoord2f(0.0, 1.0);
    glVertex2f(min[0], max[1]);

    glTexCoord2f(1.0, 1.0);
    glVertex2f(max[0], max[1]);
  }
  glEnd();
  glFlush();
  glutSwapBuffers();
}

}

std::unique_ptr<viewer> viewer::s_viewer;

void viewer::create(int* argc, char** argv) {
  if (s_viewer) {
    std::cerr << "Only one viewer may be created per application.\n";
    std::exit(-1);
  }

  glutInit(argc, argv);
  s_viewer.reset(new viewer());
}

void viewer::show() {
  if (!s_viewer) {
    std::cerr << "You must create the viewer before you can show it.\n";
    std::exit(-1);
  }

  const fractal2d* fractal = get_fractal();
  if (fractal == nullptr) {
    std::cerr << "Must set the fractal to display before showing the viewer.\n";
    std::exit(-1);
  }

  glutInitWindowSize(fractal->width(), fractal->height());
  glutCreateWindow("fractal viewer");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  s_viewer->init_texture();
  glutMainLoop();
}

void viewer::init_texture() {
  const fractal2d* fractal = get_fractal();
  assert(fractal != nullptr);

  GLuint tex_id;
  glGenTextures(1, &tex_id);

  // "Bind" the newly created texture : all future texture functions will modify
  // this texture
  glBindTexture(GL_TEXTURE_2D, tex_id);

  // Give the image to OpenGL
  const fractal2d::grid_type& G = fractal->get_grid();
  uint8_t* data = new uint8_t[G.size()];
  int idx = 0;
  for (int j = 0; j < G.height(); ++j) {
    for (int i = 0; i < G.width(); ++i) {
      data[idx++] = G(i, j)? std::numeric_limits<uint8_t>::max() : 0;
    }
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
               fractal->width(), fractal->height(), 0,
               GL_RED, GL_UNSIGNED_BYTE,
               reinterpret_cast<const void*>(data));

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  m_tex_id = tex_id;
}
