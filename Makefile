CXX=clang++
CXX_FLAGS=-std=c++20 -Iincludes -g -fstandalone-debug -O0 -Wall -Wextra -Werror

exec: bin/exec
tests: bin/tests

bin/exec: ./src/driver.cc ./src/utilities.cc ./src/wikigraph.cc
	$(CXX) $(CXX_FLAGS) $^ -o $@

bin/tests: ./tests/tests.cc ./src/utilities.cc ./src/wikigraph.cc
	$(CXX) $(CXX_FLAGS) $^ -o $@

production: ./src/driver.cc ./src/utilities.cc ./src/wikigraph.cc
	$(CXX) $(CXX_FLAGS) $^ -o ./wikigraph

.DEFAULT_GOAL := exec
.PHONY: exec tests clean

clean:
	rm -f bin/*
