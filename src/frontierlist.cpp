#include "frontierlist.h"

std::shared_ptr<Move> FrontierStack::top() {
    return list.top();
}

void FrontierStack::push(std::shared_ptr<Move> move) {
    return list.push(move);
}

void FrontierStack::pop() {
    list.pop();
}

bool FrontierStack::empty() {
    return list.empty();
}

std::shared_ptr<Move> FrontierQueue::top() {
    return list.front();
}

void FrontierQueue::push(std::shared_ptr<Move> move) {
    return list.push(move);
}

void FrontierQueue::pop() {
    list.pop();
}

bool FrontierQueue::empty() {
    return list.empty();
}