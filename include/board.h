#pragma once

#include <bitset>
#include <iostream>

#define INITIAL_BOARD 0x1fffeffff
#define OPTIMAL_BOARD 0x10000
#define CLEAR_LINES(COUNT) do { std::cout << "\033[" #COUNT "F"; } while (false)

namespace peg_solitaire {
    uint8_t translate_index(uint8_t index);
    uint8_t translate_coordinate(uint8_t coordinate);
    void print_board(std::bitset<33> board, uint64_t nodes_expanded = 0);
    void set_algorithm(std::string alg);
    std::pair<double, std::string> process_mem_usage();
    uint32_t parse_elapsed_time();
}