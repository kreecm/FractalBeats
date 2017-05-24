#include <iostream>
#include <iomanip>
#include <map>
#include <memory>
#include <string>

#include "mandelbrot.h"
#include "viewer.h"

enum fractal_type {
  kMandelbrot
};

struct fracgen_params {
  int resolution = 32;
  int max_iter = 100;
  fractal_type fractal = kMandelbrot;
  bool print = false;
  bool view = false;
};

const std::map<std::string, fractal_type> fractal_types = {
  {"mandelbrot", kMandelbrot}
};

void usage() {
  std::cout << "fracgen [OPTIONS]\n"
            << "  --fractal <type>    The type of fractal to generate.\n"
            << "  --resolution <int>  The resolution to generate.\n"
            << "  --maxiter <int>     The maximum iteration of the generator.\n"
            << "  --print             Print the result to stdout.\n"
            << "  --view              Render the fractal in a viewer.\n"
            << "  --help              Print this help message.\n";
}

void parse_params(int argc, char* argv[], fracgen_params* params) {
  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg[0] != '-') {
      std::cerr << "Unexpected argument: " << arg << "\n";
      usage();
      std::exit(-1);
    }

    if (arg == "--fractal" || arg == "-f") {
      params->fractal = fractal_types.at(argv[++i]);
    } else if (arg == "--resolution" || arg == "-r") {
      params->resolution = std::stoi(argv[++i]);
    } else if (arg == "--maxiter" || arg == "-m") {
      params->max_iter = std::stoi(argv[++i]);
    } else if (arg == "--print" || arg == "-p") {
      params->print = true;
    } else if (arg == "--view" || arg == "-v") {
      params->view = true;
    } else if (arg == "--help" || arg == "-h") {
      usage();
      std::exit(0);
    }
  }
}

int main(int argc, char* argv[]) {
  viewer::create(&argc, argv);

  fracgen_params params;
  parse_params(argc, argv, &params);

  std::unique_ptr<fractal2d> frac;
  switch (params.fractal) {
    case kMandelbrot:
      frac.reset(new mandelbrot(params.resolution, params.max_iter));
      break;
  }

  frac->generate();
  if (params.print) {
    frac->print(std::cout);
  }
  if (params.view) {
    viewer::set_fractal(frac.get());
    viewer::show();
  }

  return 0;
}
