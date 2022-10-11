CXX = clang++
CFLAGS = -g -O0 -pedantic -std=c++20 -Iinclude/ -Llib/
LIBS=-lm -ldl -lpthread -l:libmemflow_ffi.a

ODIR=./

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

main.out: main.o
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: all
all: main.out

.DEFAULT_GOAL := all

clean:
	rm -f $(ODIR)/*.o
