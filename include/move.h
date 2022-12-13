#pragma once

#include <bitset>
#include <array>
#include <map>
#include <memory>
#include <queue>

/*
    Abstarct move class defines the needed function and attributes for the generation of next 
    states. Iterator pattern is used in order to reduce the space complexity needed for the 
    dfs. Next child is only calculated when the 'next' function is called. It reduces space 
    complexity of dfs to m from m*b. Unfortunately this pattern does not reduce the space of 
    complexity of bfs due to FIFO principal. Also this pattern does not work well with heuristic
    move generation because finding best possible next state requires looking all childs. Therefore 
    heuristic move generation also does not reduce the space complexity.
*/

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