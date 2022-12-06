#include "move.h"

#include <algorithm>

#include "board.h"

Move::Move(std::bitset<33> board): board{board} {}

OrderedMove::OrderedMove(std::bitset<33> board):
    Move(board),
    current_index{1} {}

RandomMove::RandomMove(std::bitset<33> board):
    Move(board),
    current_index{0} {
    for (int i = 0; i < (int)this->perm.size(); i++) this->perm[i] = i;

    std::random_shuffle(this->perm.begin(), this->perm.end());
}

HeuristicMove::HeuristicMove(std::bitset<33> board):
    Move(board) {
    this->calculate_moves();
}

std::pair<std::bitset<33>, std::bitset<33>> Move::check_for_next_move(uint8_t peg) {
    
    std::pair<std::bitset<33>, std::bitset<33>> result {0x0, 0x0};
    std::bitset<33> next_board(this->board);
    
    if (!board[peg]) return result;

    uint8_t cd = translate_2d(peg);
    uint8_t i = cd / 7, j = cd % 7;

    uint8_t 
        left = translate_1d(j - 1 < 7 ? i * 7 + j - 1 : -1),
        top = translate_1d(i - 1 < 7 ? i * 7 + j - 7 : -1),
        right = translate_1d(j + 1 < 7 ? i * 7 + j + 1 : -1),
        bottom = translate_1d(i + 1 < 7 ? i * 7 + j + 7 : -1);

    if (left != 0xff && right != 0xff && (next_board[left] ^ next_board[right])) {
        next_board.reset(peg);
        next_board.flip(left);
        next_board.flip(right);
        result.first = next_board;
        next_board = this->board;
    }

    if (top != 0xff && bottom != 0xff && (next_board[top] ^ next_board[bottom])) {
        next_board.reset(peg);
        next_board.flip(top);
        next_board.flip(bottom);
        result.second = next_board;
    }

    if (result.first == 0x0) {
        result.first = result.second;
        result.second = 0x0;
    }

    return result;
}

std::bitset<33> OrderedMove::next() {
    if (this->board.none()) {
        this->current_index = 33;
        return 0x0;
    }

    if (cached_board != 0x0) {
        std::bitset<33> cached_board(this->cached_board);
        this->cached_board = 0x0;
        return cached_board;
    }

    while (this->current_index < 33) {

        auto next_move = this->check_for_next_move(this->current_index);

        if (next_move.first == 0x0) {
            this->current_index++;
            continue;
        }
        
        if (next_move.second != 0x0) {
            this->cached_board = next_move.second;
        }

        this->current_index++;

        return next_move.first;
    }

    return 0x0;
}

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

    uint8_t size = this->perm.size();
    while (this->current_index < size) {

        auto next_move = this->check_for_next_move(this->perm[this->current_index]);

        if (next_move.first == 0x0) {
            this->current_index++;
            continue;
        }
        
        if (next_move.second != 0x0) {
            if (rand() > RAND_MAX / 2) std::swap(next_move.first, next_move.second);

            this->cached_board = next_move.second;
        }

        this->current_index++;

        return next_move.first;
    }

    return 0x0;
}

void HeuristicMove::calculate_moves() {
    for (uint8_t i = 1; i < 33; i++) {
        auto moves = this->check_for_next_move(i);

        if (moves.first == 0x0) continue;

        this->moves[this->calculate_heuristic_score(moves.first)] = moves.first;

        if (moves.second == 0x0) continue;

        this->moves[this->calculate_heuristic_score(moves.second)] = moves.second;
    }

    this->next_move = this->moves.begin();
}

uint16_t HeuristicMove::calculate_heuristic_score(std::bitset<33> board) {
    
    uint16_t score = 0;
    for (uint8_t peg = 0; peg < 33; peg++) {
        if (!board[peg]) continue;

        uint8_t cd = translate_2d(peg);
        uint8_t i = cd / 7, j = cd % 7;

        uint8_t 
            left = translate_1d(j - 1 < 7 ? i * 7 + j - 1 : -1),
            top = translate_1d(i - 1 < 7 ? i * 7 + j - 7 : -1),
            right = translate_1d(j + 1 < 7 ? i * 7 + j + 1 : -1),
            bottom = translate_1d(i + 1 < 7 ? i * 7 + j + 7 : -1);

        uint8_t loneliness = 1;
        loneliness += !((left == 0xff) && board[left]);
        loneliness += !((top != 0xff) && board[top]);
        loneliness += !((right != 0xff) && board[right]);
        loneliness += !((bottom != 0xff) && board[bottom]);

        uint8_t distance = std::abs((int8_t)i - 3) + std::abs((int8_t)j - 3);

        score += loneliness * distance;
    }
    return score;
}

std::bitset<33> HeuristicMove::next() {
    if (this->next_move == this->moves.end()) {
        return 0x0;
    }

    auto next_move = this->next_move->second;

    this->next_move++;

    return next_move;
}