#pragma once

#include <bitset>
#include <vector>

#include "board.h"

class Move {
public:
    std::bitset<33> board;
private:
    std::bitset<33> cached_board;
    uint8_t current_index;

public:
    Move(std::bitset<33>);  
    std::bitset<33> next();
};