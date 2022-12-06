#pragma once

#include <memory>

#include "move.h"

class MoveFactory {
    public:
        virtual std::shared_ptr<Move> create_move(std::bitset<33> board) const = 0;
};

class OrderedMoveFactory : public MoveFactory{
    public:
        std::shared_ptr<Move> create_move(std::bitset<33> board) const override;
};

class RandomMoveFactory : public MoveFactory{
    public:
        std::shared_ptr<Move> create_move(std::bitset<33> board) const override;
};

class HeuristicMoveFactory : public MoveFactory{
    public:
        std::shared_ptr<Move> create_move(std::bitset<33> board) const override;
};