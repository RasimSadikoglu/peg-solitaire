#include "search.h"
#include "move.h"

#include <unordered_map>
#include <string>
#include <thread>

int main(int argc, char* argv[]) {

    std::unordered_map<std::string, void (*)()> jump_table = {
        std::make_pair("bfs", peg_solitaire::breadth_first_search),
        std::make_pair("dfs", peg_solitaire::depth_first_search),
        std::make_pair("idp", peg_solitaire::iterative_deepining_search),
        std::make_pair("dfs-random", peg_solitaire::depth_first_search_random_selection),
        std::make_pair("dfs-heuristic", peg_solitaire::depth_first_search_heuristic_selection),
    };

    peg_solitaire::set_time_limit(std::atoi(argv[2]));
    std::thread wait(peg_solitaire::start_timer);
    jump_table[argv[1]]();
    wait.join();
}