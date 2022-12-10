#include "movefactory.h"

std::shared_ptr<Move> OrderedMoveFactory::create_move(std::bitset<33> board, std::shared_ptr<Move> parent) const {
    return std::shared_ptr<Move>(new OrderedMove(board, parent));
}

std::shared_ptr<Move> RandomMoveFactory::create_move(std::bitset<33> board, std::shared_ptr<Move> parent) const {
    return std::shared_ptr<Move>(new RandomMove(board, parent));
}

std::shared_ptr<Move> HeuristicMoveFactory::create_move(std::bitset<33> board, std::shared_ptr<Move> parent) const {
    return std::shared_ptr<Move>(new HeuristicMove(board, parent));
}