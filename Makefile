CXX:=g++
CXXFLAGS:=-O3 -Wall -Wextra -Iinclude -std=c++23
MACROS = -DBYPASS_DEPTH_CHECK

DEPS:=move board search movefactory frontierlist
OBJS:=main move board search movefactory frontierlist
DIRS:=bin obj
BIN:=bin/main

_DEPS:=$(patsubst %, include/%.h, $(DEPS))
_OBJS:=$(patsubst %, obj/%.o, $(OBJS))

$(BIN): $(DIRS) $(_OBJS)
	$(CXX) $(CXXFLAGS) $(_OBJS) -o $@

obj/%.o: src/%.cpp $(_DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: bin/main
	bin/main dfs-heuristic 60

debug: bin/main
	gdb bin/main

valgrind: bin/main
	valgrind --leak-check=full --show-leak-kinds=all bin/main

$(DIRS):
	mkdir $@

.PHONY: clean
clean:
	rm -rf ${DIRS} target