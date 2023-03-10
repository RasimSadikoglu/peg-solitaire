MACROS:=-DMEMORY_LIMIT=8 -DSOLUTION_ANIMATION_SPEED=500

CXXFLAGS:=-Iinclude -std=c++14
VERBOSITY_FLAGS:=-Wall -Werror -Wextra -Wpedantic -Wconversion -Wcast-align -Wunused -Wpointer-arith -Wcast-qual -Wno-missing-braces
OPT_FLAGS:=-Ofast -finline-functions -funroll-loops

algorithm=dfs
time_limit=60

DEPS:=move board search movefactory frontierlist
OBJS:=main move board search movefactory frontierlist
DIRS:=bin obj
BIN:=bin/main

_DEPS:=$(patsubst %, include/%.h, $(DEPS))
_OBJS:=$(patsubst %, obj/%.o, $(OBJS))

$(BIN): $(DIRS) $(_OBJS)
	$(CXX) $(_OBJS) -o $@

obj/%.o: src/%.cpp $(_DEPS)
	$(CXX) $(CXXFLAGS) $(OPT_FLAGS) $(VERBOSITY_FLAGS) $(MACROS) -c $< -o $@

run: bin/main
	bin/main $(algorithm) $(time_limit)

debug: bin/main
	gdb bin/main $(algorithm) $(time_limit)

valgrind: bin/main
	valgrind --leak-check=full --show-leak-kinds=all bin/main $(algorithm) $(time_limit)

$(DIRS):
	mkdir $@

.PHONY: clean
clean:
	rm -rf ${DIRS} target
