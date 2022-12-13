#pragma once

#include <cstdint>

/*
    Public API for the search algorithms. There are 5 different search algorithm 
    that can be called without giving any argument. Arguments are given as command
    line arguments when running the program. Every one of search algorithms calls
    the '_search' function in the 'search.cpp' file. That file executes the algorithm
    using polymorphism.
*/
namespace peg_solitaire {
    void breadth_first_search();
    void depth_first_search();
    void iterative_deepining_search();
    void depth_first_search_random_selection();
    void depth_first_search_heuristic_selection();

    void set_time_limit(uint8_t minutes);

    /*
        This function does side works for our program in order to not lose precious cpu cycles
        for instructions that are not related with the search. This function is called from
        a thread and only does 3 things

        1. Checks for the prints the current status of the algorithm on 1 second intervals.
        2. Checks for the time limit, and if the time limit is exceeded changes terminate flag to 0x1.
        3. Checks for memory limit, and if the memory limit is exceeded changes terminate flag to 0x2.
    */
    void helper_thread();
}