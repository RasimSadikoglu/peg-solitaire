#pragma once

#include <memory>

#include "move.h"

/*
    Abstract factory pattern for Move classes.
*/

class MoveFactory {
    public:
        virtual std::shared_ptr<Move> create_move(std::bitset<33> board, std::shared_ptr<Move> parent) const = 0;
};

class OrderedMoveFactory : public MoveFactory {
    public:
        std::shared_ptr<Move> create_move(std::bitset<33> board, std::shared_ptr<Move> parent) const override;
};

class RandomMoveFactory : public MoveFactory {
    public:
        std::shared_ptr<Move> create_move(std::bitset<33> board, std::shared_ptr<Move> parent) const override;
};

class HeuristicMoveFactory : public MoveFactory {
    public:
        std::shared_ptr<Move> create_move(std::bitset<33> board, std::shared_ptr<Move> parent) const override;
};