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
static uint64_t maximum_number_of_nodes_in_memory = 0;

static std::shared_ptr<Move> _search(FrontierList &frontier_list, const MoveFactory &move_factory, uint64_t &expanded_nodes, const uint8_t depth = -1) {
    frontier_list.push(move_factory.create_move(INITIAL_BOARD, nullptr));
    expanded_nodes++;

    std::shared_ptr<Move> best_board = move_factory.create_move(INITIAL_BOARD, nullptr);

    while (!frontier_list.empty()) {

#ifndef BYPASS_TIME_MEMORY_LIMIT
        if (terminate) {
            break;
        }
#endif

        auto top = frontier_list.top();

        if (print) {
            CLEAR_LINES(15);
            peg_solitaire::print_board(best_board->board, time_limit, expanded_nodes, maximum_number_of_nodes_in_memory);
            print = false;
        }

#ifndef BYPASS_DEPTH_CHECK
        if (depth > 32 - top->board.count()) {
#endif
            auto next_board = top->next();

            if (next_board.any()) {
                expanded_nodes++;
                frontier_list.push(move_factory.create_move(next_board, top));
                maximum_number_of_nodes_in_memory = std::max(maximum_number_of_nodes_in_memory, frontier_list.size());
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
    }

    return best_board;
}

static void _setup_search(FrontierList &&frontier_list, const MoveFactory &&move_factory, const uint8_t depth = -1) {
    peg_solitaire::print_board(INITIAL_BOARD, time_limit);
    uint64_t expanded_nodes = 0;
    auto best_board = move_factory.create_move(INITIAL_BOARD, nullptr);
    if (depth != 0xff) for (uint8_t d = 0; d < depth && depth != 0xff; d++) {
        auto new_board = _search(frontier_list, move_factory, expanded_nodes, d);
        if (new_board->board == OPTIMAL_BOARD || new_board->board.count() < best_board->board.count()) best_board = new_board;
    } else {
        best_board = _search(frontier_list, move_factory, expanded_nodes);
    }
    CLEAR_LINES(15);
    peg_solitaire::print_board(best_board->board, time_limit, expanded_nodes, maximum_number_of_nodes_in_memory);
    peg_solitaire::print_solution(best_board, terminate);
}

namespace peg_solitaire {
    void breadth_first_search() {
        _setup_search(FrontierQueue(), OrderedMoveFactory());
    }

    void depth_first_search() {
        _setup_search(FrontierStack(), OrderedMoveFactory());
    }

    void iterative_deepining_search() {
        _setup_search(FrontierStack(), OrderedMoveFactory(), 33);
    }

    void depth_first_search_random_selection() {
        srand(time(0));
        _setup_search(FrontierStack(), RandomMoveFactory());
    }

    void depth_first_search_heuristic_selection() {
        _setup_search(FrontierStack(), HeuristicMoveFactory());
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