#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <chrono>
#include <list>
#include <fstream>


using Clock = std::chrono::steady_clock;

struct Stats{
    size_t hits = 0;
    size_t misses = 0;
    size_t evictions = 0;
    size_t size = 0;
};

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
        Stats stats() const;

        void save(const std::string& filename) const;
        void load(const std::string& filename);
        void enableAOF(const std::string& filename);
        void disableAOF();
        void loadAOF(const std::string& filename);
        void logAOF(const std::string& cmd);
    private:
        std::unordered_map<std::string, Node> data;
        mutable std::shared_mutex m;
        size_t capacity;
        std::list<std::string> lru;
        void moveToFront(const std::string& key);
        void evictIfNeeded();

        size_t hits_ = 0;
        size_t misses_ = 0;
        size_t evictions_ = 0;

        bool aof_enabled_ = false;
        std::ofstream aof_out_;
        std::string aof_filename_;
};