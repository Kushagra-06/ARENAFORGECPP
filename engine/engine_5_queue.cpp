#include "arena_engine.hpp"

namespace arena {
    void Waitlist::enqueue(int v) {
        q.push(v);
    }

    bool Waitlist::dequeue(int& out) {
        if (q.empty()) return false;
        out = q.front();
        q.pop();
        return true;
    }

    bool Waitlist::peek(int& out) const {
        if (q.empty()) return false;
        out = q.front();
        return true;
    }

    size_t Waitlist::size() const {
        return q.size();
    }
}
