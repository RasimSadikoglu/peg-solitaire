#pragma once

#include <memory>
#include <stack>
#include <queue>

#include "move.h"

class FrontierList {
    public:
        virtual std::shared_ptr<Move> top() const = 0;
        virtual void push(std::shared_ptr<Move>) = 0;
        virtual void pop() = 0;
        virtual bool empty() const = 0;
        virtual uint64_t size() const = 0;
};

class FrontierStack : public FrontierList {
    private:
        std::stack<std::shared_ptr<Move>> list;

    public:
        std::shared_ptr<Move> top() const override;
        void push(std::shared_ptr<Move>) override;
        void pop() override;
        bool empty() const override;
        uint64_t size() const override;
};

class FrontierQueue : public FrontierList {
    private:
        std::queue<std::shared_ptr<Move>> list;

    public:
        std::shared_ptr<Move> top() const override;
        void push(std::shared_ptr<Move>) override;
        void pop() override;
        bool empty() const override;
        uint64_t size() const override;
};