#include "movefactory.h"

std::shared_ptr<Move> OrderedMoveFactory::create_move(std::bitset<33> board) const {
    std::shared_ptr<Move> move(new OrderedMove(board));
    return move;
}

std::shared_ptr<Move> RandomMoveFactory::create_move(std::bitset<33> board) const {
    std::shared_ptr<Move> move(new RandomMove(board));
    return move;
}