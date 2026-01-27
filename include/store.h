#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <chrono>
#include <list>

using Clock = std::chrono::steady_clock;

struct Node{
    std::string value;
    std::optional<std::chrono::steady_clock::time_point> expire_at;
    std::list<std::string>::iterator it;
};

class KVStore{
    public: 
        explicit KVStore(size_t max_capacity); 
        void set(const std::string& key, const std::string& value);
        void set(const std::string& key, const std::string& value, std::chrono::seconds ttl);

        std::optional<std::string> get(const std::string& key);
        void del(const std::string& key);
    private:
        std::unordered_map<std::string, Node> data;
        mutable std::shared_mutex m;
        size_t capacity;
        std::list<std::string> lru;
        void moveToFront(const std::string& key);
        void evictIfNeeded();
};