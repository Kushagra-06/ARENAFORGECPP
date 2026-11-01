#include "arena_engine.hpp"

namespace arena {
    void TeamDirectory::put(const std::string& key, int value) {
        table[key] = value;
    }

    bool TeamDirectory::get(const std::string& key, int& out) const {
        auto it = table.find(key);
        if (it != table.end()) {
            out = it->second;
            return true;
        }
        return false;
    }

    bool TeamDirectory::remove(const std::string& key) {
        return table.erase(key) > 0;
    }

    size_t TeamDirectory::count() const {
        return table.size();
    }
}
