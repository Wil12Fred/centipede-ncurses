CXX      ?= g++
CXXFLAGS ?= -O2 -Wall
LDLIBS   := -lncurses

all: snake pr

snake: main.cc bala.h funciones.h game.h serpiente.h tanke.h
	$(CXX) $(CXXFLAGS) -o $@ main.cc $(LDLIBS)

pr: pr.cc
	$(CXX) $(CXXFLAGS) -o $@ pr.cc $(LDLIBS)

clean:
	rm -f snake pr

.PHONY: all clean
