#include "search.h"

#include <bitset>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <iomanip>
#include <cstdio>

#include "move.h"
#include "movefactory.h"
#include "frontierlist.h"
#include "board.h"

#define MEMORY_LIMIT 8

static uint8_t terminate = 0x0;
static bool print = false;
static uint8_t time_limit = -1;

static std::shared_ptr<Move> search(FrontierList &frontier_list, const MoveFactory &move_factory, uint64_t &cycle_count, const uint8_t depth = -1) {
    frontier_list.push(move_factory.create_move(INITIAL_BOARD, nullptr));

    std::shared_ptr<Move> best_board = move_factory.create_move(0xff, nullptr);

    while (!frontier_list.empty()) {

#ifndef BYPASS_TIME_MEMORY_LIMIT
        if (terminate) {
            break;
        }
#endif

        auto top = frontier_list.top();

        if (print) {
            CLEAR_LINES(12);
            peg_solitaire::print_board(best_board->board, cycle_count);
            print = false;
        }

#ifndef BYPASS_DEPTH_CHECK
        if (depth > 32 - top->board.count()) {
#endif
            auto next_board = top->next();

            if (next_board.any()) {
                frontier_list.push(move_factory.create_move(next_board, top));
                continue;
            }
#ifndef BYPASS_DEPTH_CHECK
        }
#endif

        auto current_board = top->board;
        if (current_board == OPTIMAL_BOARD || current_board.count() < best_board->board.count()) {
            best_board = top;
            if (best_board->board == OPTIMAL_BOARD) {
                terminate = 0xff;
                break;
            }
        }

        frontier_list.pop();
        cycle_count++;
    }

    CLEAR_LINES(12);
    peg_solitaire::print_board(best_board->board, cycle_count);

    return best_board;
}

namespace peg_solitaire {
    void breadth_first_search() {
        peg_solitaire::print_board(INITIAL_BOARD);
        auto frontier_list = FrontierQueue();
        uint64_t cycle_count = 0;
        search(frontier_list, OrderedMoveFactory(), cycle_count);
    }

    void depth_first_search() {
        peg_solitaire::print_board(INITIAL_BOARD);
        auto frontier_list = FrontierStack();
        uint64_t cycle_count = 0;
        search(frontier_list, OrderedMoveFactory(), cycle_count);
    }

    void iterative_deepining_search() {
        peg_solitaire::print_board(INITIAL_BOARD);
        uint64_t cycle_count = 0;
        for (uint8_t depth = 0; depth < 33; depth++) {
            auto frontier_list = FrontierStack(); 
            search(frontier_list, OrderedMoveFactory(), cycle_count, depth);
        }
    }

    void depth_first_search_random_selection() {
        peg_solitaire::print_board(INITIAL_BOARD);
        auto frontier_list = FrontierStack();
        uint64_t cycle_count = 0;
        search(frontier_list, RandomMoveFactory(), cycle_count);
    }

    void depth_first_search_heuristic_selection() {
        peg_solitaire::print_board(INITIAL_BOARD);
        auto frontier_list = FrontierStack();
        uint64_t cycle_count = 0;
        search(frontier_list, HeuristicMoveFactory(), cycle_count);
    }

    void depth_limited_search(uint8_t depth) {
        peg_solitaire::print_board(INITIAL_BOARD);
        auto frontier_list = FrontierStack();
        uint64_t cycle_count = 0;
        search(frontier_list, OrderedMoveFactory(), cycle_count, depth);
    }
    
    void set_time_limit(uint8_t minutes) {
        time_limit = minutes;
    }

    void helper_thread() {
        uint64_t artificial_cycles = 0;
        for (;;) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            artificial_cycles = peg_solitaire::parse_elapsed_time() / 10;

            if (terminate) break;

            if (artificial_cycles % 100 == 0) {
                print = true;
            }

            if (artificial_cycles > time_limit * 6000) {
                terminate = 0x1;
                break;
            }

            auto ram_usage = peg_solitaire::process_mem_usage();
            if (ram_usage.second == "GB" && ram_usage.first > MEMORY_LIMIT) {
                terminate = 0x2;
                break;
            }
        }
    }
}