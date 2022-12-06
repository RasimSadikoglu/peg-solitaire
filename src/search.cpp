#include "search.h"

#include <bitset>
#include <iostream>

#include "move.h"
#include "movefactory.h"
#include "frontierlist.h"
#include "board.h"

static void search(FrontierList &frontier_list, const MoveFactory &move_factory, uint64_t &cycle_count, const uint8_t depth = -1) {
    uint8_t _ignore_unused = depth;
    _ignore_unused = _ignore_unused;

    frontier_list.push(move_factory.create_move(INITIAL_BOARD));
    std::bitset<33> best_board{INITIAL_BOARD};

    while (!frontier_list.empty()) {
        
        auto top = frontier_list.top();

        if (cycle_count % 1000000 == 0) {
            CLEAR_BOARD;
            print_board(best_board);
            std::cout << cycle_count;
        }

#ifndef BYPASS_DEPTH_CHECK
        if (depth > 32 - top->board.count()) {
#endif
            auto next_board = top->next();

            if (next_board.any()) {
                frontier_list.push(move_factory.create_move(next_board));
                continue;
            }
#ifndef BYPASS_DEPTH_CHECK
        }
#endif

        auto current_board = top->board;
        if (current_board.count() <= best_board.count()) {
            best_board = current_board;
            if (best_board == 0x10000) break;
        }

        frontier_list.pop();
        cycle_count++;
    }

    CLEAR_BOARD;
    print_board(best_board);
    std::cout << cycle_count << "\n";
}

void print_solution(FrontierList &frontier_list) {
    while (!frontier_list.empty()) {
        auto top = frontier_list.top();
        print_board(top->board);
        frontier_list.pop();
    }
}

namespace peg_solitaire {
    void breadth_first_search() {
        print_board(INITIAL_BOARD);
        auto frontier_list = FrontierQueue();
        uint64_t cycle_count = 0;
        search(frontier_list, OrderedMoveFactory(), cycle_count);
    }

    void depth_first_search() {
        print_board(INITIAL_BOARD);
        auto frontier_list = FrontierStack();
        uint64_t cycle_count = 0;
        search(frontier_list, OrderedMoveFactory(), cycle_count);

        print_solution(frontier_list);
    }

    void iterative_deepining_search() {
        print_board(INITIAL_BOARD);
        uint64_t cycle_count = 0;
        for (uint8_t depth = 0; depth < 33; depth++) {
            auto frontier_list = FrontierStack(); 
            search(frontier_list, OrderedMoveFactory(), cycle_count, depth);
        }
    }

    void depth_first_search_random_selection() {
        print_board(INITIAL_BOARD);
        auto frontier_list = FrontierStack();
        uint64_t cycle_count = 0;
        search(frontier_list, RandomMoveFactory(), cycle_count);
    }

    void depth_first_search_heuristic_selection() {
        print_board(INITIAL_BOARD);
        auto frontier_list = FrontierStack();
        uint64_t cycle_count = 0;
        search(frontier_list, HeuristicMoveFactory(), cycle_count);

        print_solution(frontier_list);
    }

    void depth_limited_search(uint8_t depth) {
        print_board(INITIAL_BOARD);
        auto frontier_list = FrontierStack();
        uint64_t cycle_count = 0;
        search(frontier_list, OrderedMoveFactory(), cycle_count, depth);
    }
}