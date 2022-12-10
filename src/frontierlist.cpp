#include "frontierlist.h"

std::shared_ptr<Move> FrontierStack::top() const {
    return list.top();
}

void FrontierStack::push(std::shared_ptr<Move> move) {
    return list.push(move);
}

void FrontierStack::pop() {
    list.pop();
}

bool FrontierStack::empty() const {
    return list.empty();
}

uint64_t FrontierStack::size() const {
    return list.size();
}

std::shared_ptr<Move> FrontierQueue::top() const {
    return list.front();
}

void FrontierQueue::push(std::shared_ptr<Move> move) {
    return list.push(move);
}

void FrontierQueue::pop() {
    list.pop();
}

bool FrontierQueue::empty() const {
    return list.empty();
}

uint64_t FrontierQueue::size() const {
    return list.size();
}