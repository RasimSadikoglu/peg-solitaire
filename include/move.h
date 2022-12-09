#pragma once

#include <bitset>
#include <array>
#include <map>
#include <memory>

class Move {
    public:
        std::bitset<33> board;
        Move(std::bitset<33> board);
        virtual std::bitset<33> next() = 0;
        virtual ~Move() = default;

    protected: 
        std::pair<std::bitset<33>, std::bitset<33>> check_for_next_move(uint8_t peg);
        std::shared_ptr<Move> parent;
};

class OrderedMove: public Move {
    private:
        uint8_t current_index;
        std::bitset<33> cached_board;

    public:
        OrderedMove(std::bitset<33> board);
        std::bitset<33> next() override;
        ~OrderedMove() {}
};

class RandomMove: public Move {
    private:
        uint8_t current_index;
        std::bitset<33> cached_board;
        std::array<uint8_t, 33> perm;

    public:
        RandomMove(std::bitset<33> board);
        std::bitset<33> next() override;
        ~RandomMove() {}
};

class HeuristicMove: public Move {
    private:
        std::map<uint16_t, std::bitset<33>> moves;
        std::map<uint16_t, std::bitset<33>>::iterator next_move;

    private:
        void calculate_moves();
        uint16_t calculate_heuristic_score(std::bitset<33> board);
    public:
        HeuristicMove(std::bitset<33> board);
        std::bitset<33> next() override;
        ~HeuristicMove() {}
};