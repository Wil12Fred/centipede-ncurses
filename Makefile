CXX      ?= g++
CXXFLAGS ?= -O2 -Wall
LDLIBS   := -lncurses

all: snake pr

snake: main.cc bala.h funciones.h game.h serpiente.h tanke.h
	$(CXX) $(CXXFLAGS) -o $@ main.cc $(LDLIBS)

pr: pr.cc
	$(CXX) $(CXXFLAGS) -o $@ pr.cc $(LDLIBS)

tests/test_game: tests/test_game.cc bala.h funciones.h game.h serpiente.h tanke.h
	$(CXX) $(CXXFLAGS) -o $@ tests/test_game.cc

test: snake tests/test_game
	./tests/test_game
	./tests/smoke.sh ./snake

clean:
	rm -f snake pr tests/test_game

.PHONY: all test clean
