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

static bool terminate = false;
static uint8_t time_limit = -1;

static std::string process_mem_usage()
{
    /*
    Source
    https://gist.github.com/thirdwing/da4621eb163a886a03c5
    */

    double vm_usage     = 0.0;
    double resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;

    int i; for (i = 0; i < 2; i++) {
        if (resident_set < 1024) break;
        resident_set /= 1024;
    }

    std::ostringstream stringStream;
    stringStream << std::setw(7) << std::setprecision(1) << std::fixed << resident_set << "KMG"[i] << "B";
    return stringStream.str();
}

static void search(FrontierList &frontier_list, const MoveFactory &move_factory, uint64_t &cycle_count, const uint8_t depth = -1) {
    uint8_t _ignore_unused = depth;
    _ignore_unused = _ignore_unused;

    frontier_list.push(move_factory.create_move(INITIAL_BOARD));
    std::bitset<33> best_board{INITIAL_BOARD};

    while (!frontier_list.empty()) {

#ifndef BYPASS_TIME_LIMIT
        if (terminate) {
            break;
        }
#endif

        auto top = frontier_list.top();

        if (cycle_count % 10000 == 0) {
            CLEAR_BOARD;
            print_board(best_board);
            std::cout << "Ram Usage: " << process_mem_usage();
            // std::cout << cycle_count;
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

static void print_solution(FrontierList &frontier_list) {
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
    
    void set_time_limit(uint8_t minutes) {
        time_limit = minutes;
    }

    void start_timer() {
        std::this_thread::sleep_for(std::chrono::minutes(time_limit));
        terminate = true;
    }
}