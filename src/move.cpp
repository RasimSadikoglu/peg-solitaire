#include "move.h"

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

    return result;
}

std::bitset<33> OrderedMove::next() {
    if (cached_board != 0x0) {
        std::bitset<33> cached_board(this->cached_board);
        this->cached_board = 0x0;
        return cached_board;
    }

    while (this->current_index < 33) {

        auto next_move = this->check_for_next_move(this->current_index);

        if (next_move.first == 0x0) next_move.first = next_move.second;

        if (next_move.first == 0x0) {
            this->current_index++;
            continue;
        }

        if (next_move.first != next_move.second) {
            this->cached_board = next_move.second;
        }

        this->current_index++;

        return next_move.first;
    }

    return 0x0;
}

std::bitset<33> RandomMove::next() {
    if (cached_board != 0x0) {
        std::bitset<33> cached_board(this->cached_board);
        this->cached_board = 0x0;
        return cached_board;
    }

    while (this->current_index < 33) {

        auto next_move = this->check_for_next_move(this->perm[this->current_index]);

        if (next_move.first == 0x0) next_move.first = next_move.second;

        if (next_move.first == 0x0) {
            this->current_index++;
            continue;
        }

        if (next_move.first != next_move.second) {
            this->cached_board = next_move.second;
        }

        this->current_index++;

        return next_move.first;
    }

    return 0x0;
}