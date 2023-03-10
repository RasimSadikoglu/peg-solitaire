#include "search.h"
#include "move.h"
#include "board.h"

#include <unordered_map>
#include <string>
#include <thread>

int main(int argc, char* argv[]) {

    if (argc < 3) {
        exit(EXIT_FAILURE);
    }

    std::unordered_map<std::string, void (*)()> jump_table = {
        std::make_pair("bfs", peg_solitaire::breadth_first_search),
        std::make_pair("dfs", peg_solitaire::depth_first_search),
        std::make_pair("idp", peg_solitaire::iterative_deepining_search),
        std::make_pair("random-dfs", peg_solitaire::depth_first_search_random_selection),
        std::make_pair("heuristic-dfs", peg_solitaire::depth_first_search_heuristic_selection),
    };

    peg_solitaire::set_time_limit((uint8_t)std::atoi(argv[2]));
    std::thread helper_thread(peg_solitaire::helper_thread);

    peg_solitaire::set_algorithm(argv[1]);
    jump_table[argv[1]]();

    helper_thread.join();
}