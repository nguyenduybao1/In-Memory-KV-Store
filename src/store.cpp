#include <store.h>
#include <shared_mutex>
#include <mutex>

void KVStore::set(const std::string& key, const std::string& value){
    std::unique_lock<std::shared_mutex> lock(m);
    data[key] = value;
}

std::optional<std::string> KVStore::get(const std::string& key){
    std::shared_lock<std::shared_mutex> lock(m);
    auto it = data.find(key);
    if(it != data.end()){
        return it->second;
    }
    return std::nullopt;
}

void KVStore::del(const std::string& key){
    std::unique_lock<std::shared_mutex> lock(m);
    data.erase(key);
}
