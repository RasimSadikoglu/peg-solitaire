#include "search.h"

#include <chrono>
#include <thread>

std::bitset<33> depth_first_search() {

    std::stack<Move*> moves;
    moves.push(new Move(INITIAL_BOARD));
    std::bitset<33> best_board{INITIAL_BOARD};
    print_board(best_board);
    uint64_t cycle_count = 0;

    while (!moves.empty()) {
        cycle_count++;
        
        auto top = moves.top();

        if (cycle_count % 1000000 == 0) {
            CLEAR_BOARD;
            print_board(best_board);
            std::cout << cycle_count;
        }

        auto next_board = top->next();

        if (next_board.any()) {
            moves.push(new Move(next_board));
            continue;
        }

        auto current_board = top->board;
        if (current_board.count() <= best_board.count()) {
            best_board = current_board;
            if (best_board == 0x10000) break;
        }

        delete top;
        moves.pop();
    }

    return best_board;
}