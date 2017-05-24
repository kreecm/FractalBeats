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
#  include <GL/glew.h>
#  include <GL/gl.h>
#  include <GL/glext.h>
#  include <GL/glut.h>
#endif

namespace {

constexpr int kPositionAttr = 0;
constexpr int kTexCoordAttr = 1;

const std::string kDefaultVertexShader = R"(
#version 130
uniform mat4 projectionMatrix;

in vec3 position;
in vec2 texCoord;

out vec2 uvPosition;

void main()
{
    gl_Position = projectionMatrix * vec4(position, 1.0f);
    uvPosition = texCoord;
}
)";

const std::string kBoolGridFragShader = R"(
#version 130
uniform sampler2D texture0;

in vec2 uvPosition;

out vec4 color;

void main()
{
    color = texture(texture0, uvPosition).r * vec4(170, 170, 170, 255);
}
)";

struct viewer_program {
  int vert_shader;
  int frag_shader;
  int program;
  int proj_matrix_loc;
  int texture0_loc;

  viewer_program(
      const std::string& vertex_src,
      const std::string& fragment_src) {
    char log[256];
    int log_size;

    // Compile vertex shader
    const char* vert_src_ptr = vertex_src.c_str();
    int vert_src_size = vertex_src.size();
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_src_ptr, &vert_src_size);
    glCompileShader(vert_shader);
    glGetShaderInfoLog(vert_shader, 256, &log_size, log);
    std::cout << "Compiling vertex shader...\n" << log;

    // Compile fragment shader
    const char* frag_src_ptr = fragment_src.c_str();
    int frag_src_size = fragment_src.size();
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_src_ptr, &frag_src_size);
    glCompileShader(frag_shader);
    glGetShaderInfoLog(frag_shader, 256, &log_size, log);
    std::cout << "Compiling fragment shader...\n" << log;

    // Link shader program
    program = glCreateProgram();
    glAttachShader(program, vert_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    glGetProgramInfoLog(program, 256, &log_size, log);
    std::cout << "Linking program...\n" << log;

    // Bind attribute and uniform locations
    glBindAttribLocation(program, kPositionAttr, "position");
    glBindAttribLocation(program, kTexCoordAttr, "texCoord");
    proj_matrix_loc = glGetUniformLocation(program, "projectionMatrix");
    texture0_loc = glGetUniformLocation(program, "texture0");
    std::cout << "projectionMatrix at uniform: " << proj_matrix_loc << "\n"
              << "texture0 at uniform: " << texture0_loc << "\n";
  }

  ~viewer_program() {
    glDeleteProgram(program);
    glDeleteShader(frag_shader);
    glDeleteShader(vert_shader);
  }
};

void reshape(int w, int h) {
  const fractal2d* fractal = viewer::get_fractal();
  const fractal2d::point& min = fractal->min();
  const fractal2d::point& max = fractal->max();

  glViewport(0, 0, w, h);
}

void display() {
  static viewer_program fractal_program(
      kDefaultVertexShader,
      kBoolGridFragShader);

  const fractal2d* fractal = viewer::get_fractal();
  const int tex_id = viewer::get_texture();
  assert(fractal != nullptr);

  const fractal2d::point& min = fractal->min();
  const fractal2d::point& max = fractal->max();
  std::cout << "Rendering fractal domain " << min.transpose()
            << " x " << max.transpose() << std::endl;

  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  glUseProgram(fractal_program.program);

  Eigen::Matrix4f matrix = Eigen::Matrix4f::Identity();
  matrix <<
      2 / (max[0] - min[0]), 0, 0, -(max[0] + min[0]) / (max[0] - min[0]),
      0, 2 / (max[1] - min[1]), 0, -(max[1] + min[1]) / (max[1] - min[1]),
      0, 0, 1, 0,
      0, 0, 0, 1;
  glUniformMatrix4fv(fractal_program.proj_matrix_loc, 1, false, matrix.data());
  std::cout << "Set projection matrix:\n" << matrix << std::endl;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glUniform1i(fractal_program.texture0_loc, 0);

  glBegin(GL_TRIANGLE_STRIP);
  {
    glVertexAttrib2f(kTexCoordAttr, 0.0, 0.0);
    glVertexAttrib2f(kPositionAttr, min[0], min[1]);

    glVertexAttrib2f(kTexCoordAttr, 1.0, 0.0);
    glVertexAttrib2f(kPositionAttr, max[0], min[1]);

    glVertexAttrib2f(kTexCoordAttr, 0.0, 1.0);
    glVertexAttrib2f(kPositionAttr, min[0], max[1]);

    glVertexAttrib2f(kTexCoordAttr, 1.0, 1.0);
    glVertexAttrib2f(kPositionAttr, max[0], max[1]);
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
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
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
  s_viewer->init();
  glutMainLoop();
}

void viewer::init() {
  glewInit();

  const fractal2d* fractal = get_fractal();
  assert(fractal != nullptr);

  GLuint tex_id;
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
               fractal->width(), fractal->height(), 0,
               GL_RED, GL_UNSIGNED_BYTE,
               fractal->get_grid().data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  m_tex_id = tex_id;
}
