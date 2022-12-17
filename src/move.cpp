#include "move.h"
#include "board.h"

#include <algorithm>

Move::Move(std::bitset<33> board, std::shared_ptr<Move> parent): is_valid_solution{true}, board{board}, parent{parent} {}

OrderedMove::OrderedMove(std::bitset<33> board, std::shared_ptr<Move> parent):
    Move(board, parent),
    current_index{1} {}

RandomMove::RandomMove(std::bitset<33> board, std::shared_ptr<Move> parent):
    Move(board, parent),
    current_index{0} {
    for (uint8_t i = 0; i < this->perm.size(); i++) this->perm[i] = i;

    /*
        For random dfs iterate indices in the random order.
    */
    std::random_shuffle(this->perm.begin(), this->perm.end());
}

HeuristicMove::HeuristicMove(std::bitset<33> board, std::shared_ptr<Move> parent):
    Move(board, parent) {
    this->calculate_moves();
    is_valid_solution = !this->moves.empty();
}

/*
    For a given peg index, check whether there is/are move(s) that results in the peg with the given index
    is taken. This function checks whether there is a horizontal or a vertical move or both. Horizontal moves
    are prioritized.
*/
std::pair<std::bitset<33>, std::bitset<33>> Move::check_for_next_move(uint8_t peg) {
    
    std::pair<std::bitset<33>, std::bitset<33>> result {0x0, 0x0};
    std::bitset<33> next_board(this->board);
    
    if (!board[peg]) return result;

    uint8_t cd = peg_solitaire::translate_index(peg);
    uint8_t i = cd / 7, j = cd % 7;

    // Get neighbor indices.
    uint8_t 
        left = (uint8_t)(j - 1) < 7 ? peg_solitaire::translate_coordinate((uint8_t)(i * 7 + j - 1)) : -1,
        top = (uint8_t)(i - 1) < 7 ? peg_solitaire::translate_coordinate((uint8_t)(i * 7 + j - 7)) : -1,
        right = (uint8_t)(j + 1) < 7 ? peg_solitaire::translate_coordinate((uint8_t)(i * 7 + j + 1)) : -1,
        bottom = (uint8_t)(i + 1) < 7 ? peg_solitaire::translate_coordinate((uint8_t)(i * 7 + j + 7)) : -1;

    // Check for a horizontal move.
    if (left != 0xff && right != 0xff && (next_board[left] ^ next_board[right])) {
        next_board.reset(peg);
        next_board.flip(left);
        next_board.flip(right);
        result.first = next_board;
        next_board = this->board;
    }

    // Check for a vertical move.
    if (top != 0xff && bottom != 0xff && (next_board[top] ^ next_board[bottom])) {
        next_board.reset(peg);
        next_board.flip(top);
        next_board.flip(bottom);
        result.second = next_board;
    }

    // If there is only vertical move put the result in the first slot.
    if (result.first == 0x0) {
        result.first = result.second;
        result.second = 0x0;
    }

    return result;
}

/*
    Sequentially call 'check_for_next_move' function to get next moves. If there are multiple
    moves put the second one to the cache.
*/
std::bitset<33> OrderedMove::next() {
    // If board is empty board that is equivalent to a 'null' just return empty board in order to
    // save precious clock cycles.
    if (this->board.none()) {
        this->current_index = 33;
        return 0x0;
    }

    // If there is cached board present return that board.
    if (cached_board != 0x0) {
        std::bitset<33> cached_board(this->cached_board);
        this->cached_board = 0x0;
        return cached_board;
    }

    // Iterate through peg indices until a new move is found.
    while (this->current_index < 33) {

        auto next_move = this->check_for_next_move(this->current_index);

        if (next_move.first == 0x0) {
            this->current_index++;
            continue;
        }

        // Set 'is_valid_solution' to false because there are possible moves that makes this
        // board not a solution.
        if (is_valid_solution) is_valid_solution = false;
        
        if (next_move.second != 0x0) {
            this->cached_board = next_move.second;
        }

        this->current_index++;

        return next_move.first;
    }

    return 0x0;
}

/*
    This works exactly like Ordered::Move.next(). The differences are it iterates indices in random order.
    Also randomly prioritizes horizontal and vertical moves.
*/
std::bitset<33> RandomMove::next() {
    if (this->board.none()) {
        this->current_index = 33;
        return 0x0;
    }

    if (cached_board != 0x0) {
        std::bitset<33> cached_board(this->cached_board);
        this->cached_board = 0x0;
        return cached_board;
    }

    uint8_t size = (uint8_t)this->perm.size();
    while (this->current_index < size) {

        auto next_move = this->check_for_next_move(this->perm[this->current_index]);

        if (next_move.first == 0x0) {
            this->current_index++;
            continue;
        }

        if (is_valid_solution) is_valid_solution = false;
        
        if (next_move.second != 0x0) {
            if (rand() > RAND_MAX / 2) std::swap(next_move.first, next_move.second);

            this->cached_board = next_move.second;
        }

        this->current_index++;

        return next_move.first;
    }

    return 0x0;
}

/*
    This function calculates every possible move for the current state of the board and puts them into a priority
    queue.
*/
void HeuristicMove::calculate_moves() {
    for (uint8_t i = 1; i < 33; i++) {
        auto next_move = this->check_for_next_move(i);

        if (next_move.first == 0x0) continue;

        this->moves.push({this->calculate_heuristic_score(next_move.first), i, next_move.first.to_ullong()});

        if (next_move.second == 0x0) continue;

        this->moves.push({this->calculate_heuristic_score(next_move.second), i, next_move.second.to_ullong()});
    }
}

/*
    Heuristic score is calculated by checking for every peg for it's neighbors. A peg gets 1 loneliness score
    for it's every missing neighbor. Heuristic score for a board is sum of it's pegs' loneliness scores.
*/
uint16_t HeuristicMove::calculate_heuristic_score(std::bitset<33> board) {
    
    uint16_t score = 0;
    for (uint8_t peg = 0; peg < 33; peg++) {
        if (!board[peg]) continue;

        uint8_t cd = peg_solitaire::translate_index(peg);
        uint8_t i = cd / 7, j = cd % 7;

        uint8_t 
            left = (uint8_t)(j - 1) < 7 ? peg_solitaire::translate_coordinate((uint8_t)(i * 7 + j - 1)) : -1,
            top = (uint8_t)(i - 1) < 7 ? peg_solitaire::translate_coordinate((uint8_t)(i * 7 + j - 7)) : -1,
            right = (uint8_t)(j + 1) < 7 ? peg_solitaire::translate_coordinate((uint8_t)(i * 7 + j + 1)) : -1,
            bottom = (uint8_t)(i + 1) < 7 ? peg_solitaire::translate_coordinate((uint8_t)(i * 7 + j + 7)) : -1;

        uint8_t loneliness = 0;
        loneliness += !((left != 0xff) && board[left]);
        loneliness += !((top != 0xff) && board[top]);
        loneliness += !((right != 0xff) && board[right]);
        loneliness += !((bottom != 0xff) && board[bottom]);

        score += loneliness;
    }
    return score;
}

std::bitset<33> HeuristicMove::next() {
    if (this->moves.empty()) return 0x0;

    auto next_move = this->moves.top()[2];

    this->moves.pop();

    return next_move;
}