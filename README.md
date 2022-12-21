# C++ Solution for Peg Solitaire

# How to run

*make* and any *c++ compiler (e.g. g++)* are required.

## Running with make

```
make run algorithm=dfs time_limit=60
```

## Running without make

```
mkdir bin
g++ -Iinclude -Ofast -std=c++14 src/board.cpp src/frontierlist.cpp src/main.cpp src/move.cpp src/movefactory.cpp src/search.cpp -o bin/main
bin/main dfs 60
```

---

```
algorithms = [dfs, bfs, idp, random-dfs, heuristic-dfs]
time_limits = Any number between 1 to 255 (in minutes)
```

---

## Macros
These macros could be found on the first line of the makefile.

**MEMORY_LIMIT**

This defines the memory limit for the program that if memory usage exceeds this limit, program will be terminated. (in GB) (default is 8)

**SOLUTION_ANIMATION_SPEED**

This defines the speed of the animation when the solution is found. (in ms) (default is 500)

---

## Important Notes

* This program is intended to be compiled in Linux and it is not compatible with Windows.
* After changing value of any macro from Makefile, make sure to clear binary and object files *(make clean)*, before using *make run*.