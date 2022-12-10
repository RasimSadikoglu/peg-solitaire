#pragma once

#include <bitset>
#include <array>
#include <map>
#include <memory>
#include <queue>

class Move {
    public:
        std::bitset<33> board;
        std::shared_ptr<Move> parent;
        Move(std::bitset<33> board, std::shared_ptr<Move> parent);
        virtual std::bitset<33> next() = 0;
        virtual ~Move() = default;

    protected: 
        std::pair<std::bitset<33>, std::bitset<33>> check_for_next_move(uint8_t peg);
};

class OrderedMove: public Move {
    private:
        uint8_t current_index;
        std::bitset<33> cached_board;

    public:
        OrderedMove(std::bitset<33> board, std::shared_ptr<Move> parent);
        std::bitset<33> next() override;
        ~OrderedMove() {}
};

class RandomMove: public Move {
    private:
        uint8_t current_index;
        std::bitset<33> cached_board;
        std::array<uint8_t, 33> perm;

    public:
        RandomMove(std::bitset<33> board, std::shared_ptr<Move> parent);
        std::bitset<33> next() override;
        ~RandomMove() {}
};

class HeuristicMove: public Move {
    private:
        std::priority_queue<std::vector<uint64_t>, std::vector<std::vector<uint64_t>>, std::greater<std::vector<uint64_t>>> moves;

    private:
        void calculate_moves();
        uint16_t calculate_heuristic_score(std::bitset<33> board);
    public:
        HeuristicMove(std::bitset<33> board, std::shared_ptr<Move> parent);
        std::bitset<33> next() override;
        ~HeuristicMove() {}
};