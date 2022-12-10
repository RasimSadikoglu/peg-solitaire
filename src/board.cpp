#include "board.h"

#include <iomanip>
#include <array>
#include <chrono>
#include <unordered_map>
#include <fstream>
#include <stack>
#include <thread>
#include <cstdio>
#include <iostream>

#if !(defined(_WIN32) || defined(_WIN64))
#include <unistd.h>
#endif

static const std::array<const uint8_t, 33> _index_lookup = {
             2,  3,  4,
             9, 10, 11,
    14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27,
    28, 29, 30, 31, 32, 33, 34,
            37, 38, 39,
            44, 45, 46
};

static const std::array<const uint8_t, 49> _coordinate_lookup = {
    0xff, 0xff,    0,    1,    2, 0xff, 0xff,
    0xff, 0xff,    3,    4,    5, 0xff, 0xff,
       6,    7,    8,    9,   10,   11,   12,
      13,   14,   15,   16,   17,   18,   19,
      20,   21,   22,   23,   24,   25,   26,
    0xff, 0xff,   27,   28,   29, 0xff, 0xff,
    0xff, 0xff,   30,   31,   32, 0xff, 0xff
};

static const std::unordered_map<std::string, std::string> algorithm_full_names = {
    {"bfs", "Breadth-First Search"},
    {"dfs", "Depth-First Search"},
    {"idp", "Iterative Deepening Search"},
    {"dfs-random", "Depth-First Search with Random Selection"},
    {"dfs-heuristic", "Depth-First Search with a Node Selection Heuristic"}
};

static std::string algorithm;
static std::chrono::_V2::steady_clock::time_point begin;

static void _print_board(std::bitset<33> board) {
    int bi = 0;
    int c = peg_solitaire::translate_index(bi);

    for (int i = 0; i < 7; i++) {
        std::printf("%4s", "");
        for (int j = 0; j < 7; j++) {
            if (c / 7 == i && c % 7 == j) {
                std::printf(" %s ", board[bi] ? "o" : " ");
                c = peg_solitaire::translate_index(++bi);
            } else {
                std::printf(" - ");
            }
        }
        std::printf("\n");
    }
    std::printf("\n");
} 

namespace peg_solitaire {

    uint8_t translate_index(uint8_t index) {
        return index < 33 ? _index_lookup[index] : -1;
    }

    uint8_t translate_coordinate(uint8_t coordinate) {
        return coordinate < 49 ? _coordinate_lookup[coordinate] : -1;
    }

    void print_board(std::bitset<33> board, uint8_t time_limit, uint64_t nodes_expanded, uint64_t maximum_nodes) {
        std::printf("%s - %d Minute%s\n\n", algorithm.c_str(), (int)time_limit, time_limit == 1 ? "" : "s");

        _print_board(board);

        std::printf("Remaining pieces: %10ld\n", board.count());
        std::printf("Expanded Nodes: %12ld\n", nodes_expanded);
        std::printf("Maximum nodes: %13ld\n", maximum_nodes);

        auto ram_usage = peg_solitaire::process_mem_usage();
        std::printf("Ram usage: %15.1lf%s\n", ram_usage.first, ram_usage.second.c_str());

        auto elapsed_time = peg_solitaire::parse_elapsed_time();
        uint16_t 
            milliseconds = elapsed_time % 1000,
            seconds = (elapsed_time / 1000) % 60,
            minutes = (elapsed_time / 60000) % 60,
            hours = (elapsed_time / 3600000) % 60;
        std::printf("%-16s%02d:%02d:%02d:%03d\n", "Time:", hours, minutes, seconds, milliseconds);
    }

    void print_solution(std::shared_ptr<Move> best_board, uint8_t status) {
        bool is_valid_solution = !(new OrderedMove(best_board->board, nullptr))->next().any();

        if (!is_valid_solution && status == 0x1) {
            std::printf("No solution found - Time Limit\n");
            return;
        }

        if (!is_valid_solution && status == 0x2) {
            std::printf("No solution found - Out of Memory\n");
            return;
        }

        if (status == 0xff) {
            std::printf("Optimum solution found");
        } else if (status == 0x1) {
            std::printf("Sub-optimum Solution Found with %ld remaining pegs - Time Limit", best_board->board.count());
        } else if (status == 0x2) {
            std::printf("Sub-optimum Solution Found with %ld remaining pegs - Out of Memory", best_board->board.count());
        }

        std::printf("\nDo you want to print the solution? (y/n): ");

        std::string choice;
        do {
            std::cin >> choice;
        } while (choice != "y" && choice != "n");

        if (choice == "n") return;

        std::stack<std::bitset<33>> solution;

        while (best_board != nullptr) {
            solution.push(best_board->board);
            best_board = best_board->parent;
        }

        std::printf("\n");
        while (!solution.empty()) {
            _print_board(solution.top());
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            solution.pop();
            if (!solution.empty()) CLEAR_LINES(8);
        }
    }

    void set_algorithm(std::string alg) {
        algorithm = algorithm_full_names.at(alg);
        begin = std::chrono::steady_clock::now();
    }

    std::pair<double, std::string> process_mem_usage() {
        /*
        Source
        https://gist.github.com/thirdwing/da4621eb163a886a03c5
        */
#if !(defined(_WIN32) || defined(_WIN64))
        double vm_usage     = 0.0;

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

        vm_usage = vsize / 1024.0;
        int i; for (i = 0; i < 2; i++) {
            if (vm_usage < 1024) break;
            vm_usage /= 1024;
        }

        return {vm_usage, std::string(1, "KMG"[i]) + "B"};
#elif
        return {0, "KB"};
#endif
    }

    uint64_t parse_elapsed_time() {
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    }
}