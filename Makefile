CXX = g++
MPICXX = mpicxx
CXXFLAGS = -O3

.PHONY: all
all: mandelbrot_seq mandelbrot_parallel

mandelbrot_seq: mandelbrot_seq.cpp
	$(CXX) $(CXXFLAGS) -o mandelbrot_seq  mandelbrot_seq.cpp

mandelbrot_parallel: mandelbrot_parallel.cpp
	$(MPICXX) $(CXXFLAGS) -o mandelbrot_parallel  mandelbrot_parallel.cpp

.PHONY: clean
clean:
	rm -f mandelbrot_seq mandelbrot_parallel
