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

static std::shared_ptr<Move> search(FrontierList &frontier_list, const MoveFactory &move_factory, uint64_t &expanded_nodes, const uint8_t depth = -1) {
    frontier_list.push(move_factory.create_move(INITIAL_BOARD, nullptr));
    expanded_nodes++;

    std::shared_ptr<Move> best_board = move_factory.create_move(0x1ffffffff, nullptr);

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

namespace peg_solitaire {
    void breadth_first_search() {
        peg_solitaire::print_board(INITIAL_BOARD, time_limit);
        auto frontier_list = FrontierQueue();
        uint64_t expanded_nodes = 0;
        auto best_board = search(frontier_list, OrderedMoveFactory(), expanded_nodes);
        CLEAR_LINES(15);
        peg_solitaire::print_board(best_board->board, time_limit, expanded_nodes, maximum_number_of_nodes_in_memory);
        peg_solitaire::print_solution(best_board, terminate);
    }

    void depth_first_search() {
        peg_solitaire::print_board(INITIAL_BOARD, time_limit);
        auto frontier_list = FrontierStack();
        uint64_t expanded_nodes = 0;
        auto best_board = search(frontier_list, OrderedMoveFactory(), expanded_nodes);
        CLEAR_LINES(15);
        peg_solitaire::print_board(best_board->board, time_limit, expanded_nodes, maximum_number_of_nodes_in_memory);
        peg_solitaire::print_solution(best_board, terminate);
    }

    void iterative_deepining_search() {
        peg_solitaire::print_board(INITIAL_BOARD, time_limit);
        uint64_t expanded_nodes = 0;
        auto best_board = std::shared_ptr<Move>(new OrderedMove(INITIAL_BOARD, nullptr));
        for (uint8_t depth = 0; depth < 33; depth++) {
            auto frontier_list = FrontierStack(); 
            auto new_board = search(frontier_list, OrderedMoveFactory(), expanded_nodes, depth);
            if (new_board->board == OPTIMAL_BOARD || new_board->board.count() < best_board->board.count()) best_board = new_board;
        }
        CLEAR_LINES(15);
        peg_solitaire::print_board(best_board->board, time_limit, expanded_nodes, maximum_number_of_nodes_in_memory);
        peg_solitaire::print_solution(best_board, terminate);
    }

    void depth_first_search_random_selection() {
        peg_solitaire::print_board(INITIAL_BOARD, time_limit);
        auto frontier_list = FrontierStack();
        uint64_t expanded_nodes = 0;
        auto best_board = search(frontier_list, RandomMoveFactory(), expanded_nodes);
        CLEAR_LINES(15);
        peg_solitaire::print_board(best_board->board, time_limit, expanded_nodes, maximum_number_of_nodes_in_memory);
        peg_solitaire::print_solution(best_board, terminate);
    }

    void depth_first_search_heuristic_selection() {
        peg_solitaire::print_board(INITIAL_BOARD, time_limit);
        auto frontier_list = FrontierStack();
        uint64_t expanded_nodes = 0;
        auto best_board = search(frontier_list, HeuristicMoveFactory(), expanded_nodes);
        CLEAR_LINES(15);
        peg_solitaire::print_board(best_board->board, time_limit, expanded_nodes, maximum_number_of_nodes_in_memory);
        peg_solitaire::print_solution(best_board, terminate);
    }

    void depth_limited_search(uint8_t depth) {
        peg_solitaire::print_board(INITIAL_BOARD, time_limit);
        auto frontier_list = FrontierStack();
        uint64_t expanded_nodes = 0;
        auto best_board = search(frontier_list, OrderedMoveFactory(), expanded_nodes, depth);
        CLEAR_LINES(15);
        peg_solitaire::print_board(best_board->board, time_limit, expanded_nodes, maximum_number_of_nodes_in_memory);
        peg_solitaire::print_solution(best_board, terminate);
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