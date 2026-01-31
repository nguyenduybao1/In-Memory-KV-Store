#pragma once
#include "store.h"
#include <vector>
#include <memory>
#include <functional>

class ShardedKVStore {
public:
    ShardedKVStore(size_t shards, size_t capacity_per_shard);

    void set(const std::string& key, const std::string& value);
    void set(const std::string& key,
             const std::string& value,
             std::chrono::seconds ttl);

    std::optional<std::string> get(const std::string& key);
    void del(const std::string& key);

    Stats stats() const;

private:
    size_t shardIndex(const std::string& key) const;

    std::vector<std::unique_ptr<KVStore>> shards_;
    size_t shard_count_;
    std::hash<std::string> hasher_;
};
