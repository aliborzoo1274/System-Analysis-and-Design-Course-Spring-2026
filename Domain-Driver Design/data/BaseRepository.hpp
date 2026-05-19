#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

using namespace std;

template <typename T>
class BaseRepository {
public:
    shared_ptr<T> findById(int id) const {
        auto it = items.find(id);
        if (it != items.end()) {
            return it->second;
        }
        return nullptr;
    }

    vector<shared_ptr<T>> findAll() const {
        vector<shared_ptr<T>> result;
        for (const auto& pair : items) {
            result.push_back(pair.second);
        }
        return result;
    }

    void save(const shared_ptr<T>& item) {
        if (!item) {
            return;
        }
        items[item->getId()] = item;
    }

    void remove(int id) {
        items.erase(id);
    }

protected:
    unordered_map<int, shared_ptr<T>> items;

};
