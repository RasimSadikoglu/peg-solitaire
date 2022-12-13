#pragma once

#include <bitset>
#include <memory>

#include "move.h"

#define INITIAL_BOARD 0x1fffeffff
#define OPTIMAL_BOARD 0x10000
#define CLEAR_LINES(COUNT) do { std::printf("\033[" #COUNT "F"); } while (false)

namespace peg_solitaire {
    /*
        This function turns given peg index into a coordinate index.

                     0,  1,  2,                                  2,  3,  4,
                     3,  4,  5,                                  9, 10, 11,
             6,  7,  8,  9, 10, 11, 12,    -------\     14, 15, 16, 17, 18, 19, 20,
            13, 14, 15, 16, 17, 18, 19,    ------- \    21, 22, 23, 24, 25, 26, 27,
            20, 21, 22, 23, 24, 25, 26,    -------/     28, 29, 30, 31, 32, 33, 34,
                    27, 28, 29,                                 37, 38, 39,
                    30, 31, 32                                  44, 45, 46


        Coordinate index is equivalent to the index of 7x7 array. It is reduced 
        to single dimensional representation.

        e.g. line i = 3, column j = 4 ==> i * 7 + j = 25 
    */
    uint8_t translate_index(uint8_t index);
    
    /*
        Does the exact opposite of the operation done on function above.
    */
    uint8_t translate_coordinate(uint8_t coordinate);
    
    void print_board(std::bitset<33> board, uint8_t time_limit = 0, uint64_t nodes_expanded = 0, uint64_t maximum_nodes = 0);
    void print_solution(std::shared_ptr<Move> best_board, uint8_t status);
    void set_algorithm(std::string alg);

    /*
        This function is not written by me, it is taken from the internet.

        Source: https://gist.github.com/thirdwing/da4621eb163a886a03c5
    */
    std::pair<double, std::string> process_mem_usage();
    uint64_t parse_elapsed_time();
}