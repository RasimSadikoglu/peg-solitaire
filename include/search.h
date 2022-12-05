#pragma once

#include <cstdint>

namespace peg_solitaire {
    void breadth_first_search();
    void depth_first_search();
    void iterative_deepining_search();
    void depth_first_search_random_selection();
    void depth_first_search_heuristic_selection();

    // extra
    void depth_limited_search(uint8_t depth);
}