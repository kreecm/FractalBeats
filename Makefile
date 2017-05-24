EIGEN_DIR=/usr/include/eigen3
INCLUDES = -I$(EIGEN_DIR)
LIBS = -lglut -lGL -lGLU -lGLEW -lXmu -lXext -lX11 -lm

CCFLAGS = -g -std=c++11 $(INCLUDES)

SRCS = 	fractal2d.cpp \
	mandelbrot.cpp \
	viewer.cpp
OBJS = $(SRCS:.cpp=.o)

all: fracgen

.cpp.o:
	$(CXX) $(CCFLAGS) -c $< -o $@

libfractal.a: $(OBJS)
	echo $(OBJS)
	ar rcs $@ $(OBJS)

fracgen: fracgen.cpp libfractal.a
	$(CXX) $(CCFLAGS) -o $@ fracgen.cpp libfractal.a $(LIBS)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f $(OBJS) libfractal.a Makefile.bak

