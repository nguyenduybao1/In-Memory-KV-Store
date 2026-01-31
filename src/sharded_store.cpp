#include "sharded_store.h"

ShardedKVStore::ShardedKVStore(size_t shards,
                               size_t capacity_per_shard)
    : shard_count_(shards)
{
    shards_.reserve(shards);

    for (size_t i = 0; i < shards; ++i) {
        shards_.push_back(
            std::make_unique<KVStore>(capacity_per_shard)
        );
    }
}

size_t ShardedKVStore::shardIndex(const std::string& key) const {
    return hasher_(key) % shard_count_;
}

void ShardedKVStore::set(const std::string& key,
                         const std::string& value) {
    shards_[shardIndex(key)]->set(key, value);
}

void ShardedKVStore::set(const std::string& key,
                         const std::string& value,
                         std::chrono::seconds ttl) {
    shards_[shardIndex(key)]->set(key, value, ttl);
}

std::optional<std::string>
ShardedKVStore::get(const std::string& key) {
    return shards_[shardIndex(key)]->get(key);
}

void ShardedKVStore::del(const std::string& key) {
    shards_[shardIndex(key)]->del(key);
}

Stats ShardedKVStore::stats() const {
    Stats total{};

    for (const auto& s : shards_) {
        auto st = s->stats();
        total.hits += st.hits;
        total.misses += st.misses;
        total.evictions += st.evictions;
        total.size += st.size;
    }

    return total;
}
