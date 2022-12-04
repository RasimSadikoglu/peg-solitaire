#include "move.h"

Move::Move(std::bitset<33> board): 
    board{board}, 
    current_index{1} {}

std::bitset<33> Move::next() {
    if (cached_board != 0x0) {
        std::bitset<33> cached_board(this->cached_board);
        this->cached_board = 0x0;
        return cached_board;
    }

    bool move_found = false;
    std::bitset<33> next_board(this->board);
    while (!move_found && this->current_index < 33) {
        if (!board[this->current_index]) {
            this->current_index++;
            continue;
        }

        uint8_t cd = translate_2d(this->current_index);
        uint8_t i = cd / 7, j = cd % 7;

        uint8_t 
            left = translate_1d(j - 1 < 7 ? i * 7 + j - 1 : -1),
            top = translate_1d(i - 1 < 7 ? i * 7 + j - 7 : -1),
            right = translate_1d(j + 1 < 7 ? i * 7 + j + 1 : -1),
            bottom = translate_1d(i + 1 < 7 ? i * 7 + j + 7 : -1);

        if (top != 0xff && bottom != 0xff && (next_board[top] ^ next_board[bottom])) {
            next_board.reset(this->current_index);
            next_board.flip(top);
            next_board.flip(bottom);
            move_found = true;
        }

        if (left != 0xff && right != 0xff && (next_board[left] ^ next_board[right])) {
            if (next_board != this->board) {
                this->cached_board = next_board;
                next_board = this->board;
            }

            next_board.reset(this->current_index);
            next_board.flip(left);
            next_board.flip(right);
            move_found = true;
        }

        this->current_index++;
    }

    return this->current_index == 33 ? 0x0 : next_board;
}