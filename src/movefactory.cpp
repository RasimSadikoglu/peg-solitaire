#include "movefactory.h"

std::shared_ptr<Move> OrderedMoveFactory::create_move(std::bitset<33> board, std::shared_ptr<Move> parent) const {
    return std::make_shared<OrderedMove>(OrderedMove(board, parent));
}

std::shared_ptr<Move> RandomMoveFactory::create_move(std::bitset<33> board, std::shared_ptr<Move> parent) const {
    return std::make_shared<RandomMove>(RandomMove(board, parent));
}

std::shared_ptr<Move> HeuristicMoveFactory::create_move(std::bitset<33> board, std::shared_ptr<Move> parent) const {
    return std::make_shared<HeuristicMove>(HeuristicMove(board, parent));
}