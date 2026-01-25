#include "store.h"
#include <shared_mutex>
#include <mutex>

using Clock = std::chrono::steady_clock;

void KVStore::set(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock(m);
    data[key] = {value, std::nullopt};
}

void KVStore::set(const std::string& key,
                  const std::string& value,
                  std::chrono::seconds ttl) {
    std::unique_lock<std::shared_mutex> lock(m);
    data[key] = {value, Clock::now() + ttl};
}

std::optional<std::string> KVStore::get(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(m); 

    auto it = data.find(key);
    if (it == data.end())
        return std::nullopt;

    if (it->second.expire_at &&
        Clock::now() >= *(it->second.expire_at)) {
        data.erase(it);
        return std::nullopt;
    }

    return it->second.value;
}

void KVStore::del(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(m);
    data.erase(key);
}
