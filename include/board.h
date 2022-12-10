#pragma once

#include <bitset>
#include <memory>

#include "move.h"

#define INITIAL_BOARD 0x1fffeffff
#define OPTIMAL_BOARD 0x10000
#define CLEAR_LINES(COUNT) do { std::printf("\033[" #COUNT "F"); } while (false)

namespace peg_solitaire {
    uint8_t translate_index(uint8_t index);
    uint8_t translate_coordinate(uint8_t coordinate);
    void print_board(std::bitset<33> board, uint8_t time_limit = 0, uint64_t nodes_expanded = 0, uint64_t maximum_nodes = 0);
    void print_solution(std::shared_ptr<Move> best_board, uint8_t status);
    void set_algorithm(std::string alg);
    std::pair<double, std::string> process_mem_usage();
    uint64_t parse_elapsed_time();
}