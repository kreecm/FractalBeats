EIGEN_DIR=/usr/include/eigen3
INCLUDES = -I$(EIGEN_DIR)
CCFLAGS = -g -std=c++11
SRCS = 	fractal2d.cpp \
	mandelbrot.cpp
OBJS = $(SRCS:.cpp=.o)

all: libfractal.a

.cpp.o:
	$(CXX) $(INCLUDES) $(CCFLAGS) -c $< -o $@

libfractal.a: $(OBJS)
	echo $(OBJS)
	ar rcs $@ $(OBJS)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f $(OBJS) libfractal.a Makefile.bak

