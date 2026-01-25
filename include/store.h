#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <chrono>

struct Entry{
    std::string value;
    std::optional<std::chrono::steady_clock::time_point> expire_at;
};

class KVStore{
    public: 
        void set(const std::string& key, const std::string& value);
        void set(const std::string& key, const std::string& value, std::chrono::seconds ttl);

        std::optional<std::string> get(const std::string& key);
        void del(const std::string& key);
    private:
        std::unordered_map<std::string, Entry> data;
        mutable std::shared_mutex m;
};