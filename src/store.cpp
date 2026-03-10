#include "store.h"
#include <shared_mutex>
#include <mutex>
#include <fstream>
#include <sstream>

using Clock = std::chrono::steady_clock;



void KVStore::set(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock(m);
    auto it = data.find(key);
    if(it != data.end()){
        it->second.value = value;
        it->second.expire_at = std::nullopt;

        moveToFront(key);
      
        logAOF("SET " + key + " " + value);
        return;
    }

    evictIfNeeded();
    lru.push_front(key);
    data[key] = {
        value,
        std::nullopt,
        lru.begin()
    };
    
   logAOF("SET " + key + " " + value);
}

void KVStore::set(const std::string& key,
                  const std::string& value,
                  std::chrono::seconds ttl) {
    std::unique_lock<std::shared_mutex> lock(m);
    auto expireTime = Clock::now() + ttl;
    auto it = data.find(key);
    if(it != data.end()){
        it->second.value = value;
        it->second.expire_at = expireTime;

        moveToFront(key);

        logAOF("SETEX " + key + " " + std::to_string(ttl.count()) + " " + value);
        return;
    } 

    evictIfNeeded();
    lru.push_front(key);
    data[key] = {
        value,
        expireTime,
        lru.begin()
    };

    logAOF("SETEX " + key + " " + std::to_string(ttl.count()) + " " + value);
}
void KVStore::setNoLog(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_mutex> lock(m);
    auto it = data.find(key);
    if(it != data.end()){
        it->second.value     = value;
        it->second.expire_at = std::nullopt;
        moveToFront(key);
        return;
    }
    evictIfNeeded();
    lru.push_front(key);
    data[key] = { value, std::nullopt, lru.begin() };
}

void KVStore::setNoLog(const std::string& key,
                       const std::string& value,
                       std::chrono::seconds ttl) {
    std::unique_lock<std::shared_mutex> lock(m);
    auto expireTime = Clock::now() + ttl;
    auto it = data.find(key);
    if(it != data.end()){
        it->second.value     = value;
        it->second.expire_at = expireTime;
        moveToFront(key);
        return;
    }
    evictIfNeeded();
    lru.push_front(key);
    data[key] = { value, expireTime, lru.begin() };
}

void KVStore::delNoLog(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(m);
    auto it = data.find(key);
    if(it == data.end()) return;
    lru.erase(it->second.it);
    data.erase(it);
}

std::optional<std::string> KVStore::get(const std::string& key) {
        {
        std::shared_lock<std::shared_mutex> lock(m);
        auto it = data.find(key);

        if(it == data.end()) {
            ++misses_;
            return std::nullopt;
        }

        if(!it->second.expire_at || Clock::now() < *(it->second.expire_at)) {
            std::string val = it->second.value;
            lock.unlock();

            std::unique_lock<std::shared_mutex> lock(m);
            if(data.count(key)) moveToFront(key);

            ++hits_;
            return val;
        }
    }

    {
        std::unique_lock<std::shared_mutex> lock(m);
        auto it = data.find(key);
        if(it != data.end() &&
           it->second.expire_at &&
           Clock::now() >= *(it->second.expire_at))
        {
            lru.erase(it->second.it);
            data.erase(it);
        }
    }

    ++misses_;
    return std::nullopt;
}

void KVStore::del(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(m);

    auto it = data.find(key);
    if(it == data.end())
        return;

    lru.erase(it->second.it);
    data.erase(it);
    logAOF("DEL " + key);
}

KVStore::KVStore(size_t cap) : capacity(cap) {}

void KVStore::moveToFront(const std::string& key){
    auto it = data.find(key);
    auto& node = it->second;

    lru.erase(node.it);
    lru.push_front(key);
    node.it = lru.begin();
}

void KVStore::evictIfNeeded(){
    if(capacity == 0 || data.size() < capacity){
        return;
    }
    for(auto rit = lru.rbegin(); rit != lru.rend(); ++rit){
        auto it = data.find(*rit);
        if(it != data.end() && it->second.expire_at && Clock::now() >= *(it->second.expire_at)){
            ++evictions_;
            data.erase(it);
            lru.erase(std::next(rit).base());
            return;
        }
    }

    
    ++evictions_;
    const std::string& oldKey = lru.back();

    data.erase(oldKey);
    lru.pop_back();

}

Stats KVStore::stats() const{
    std::shared_lock<std::shared_mutex> lock(m);
    return Stats{
        hits_.load(),     
        misses_.load(),    
        evictions_.load(), 
        data.size()
    };
}

void KVStore::save(const std::string& filename) const {
    std::shared_lock<std::shared_mutex> lock(m);
    std::ofstream out(filename);

    for(const auto& [key, node] : data){
        long long expire = -1;

        if(node.expire_at){
            expire = std::chrono::duration_cast<std::chrono::seconds>(
                node.expire_at->time_since_epoch()
            ).count();
        }

        out << key << " "
            << node.value.size() << " "
            << node.value << " "
            << expire << "\n";
    }
}

void KVStore::load(const std::string& filename){
    std::unique_lock<std::shared_mutex> lock(m);
    std::ifstream in(filename);

    data.clear();
    lru.clear();

    std::string key,value;
    long long expire;
    size_t vlen;

    while(in >> key >> vlen){
        in.ignore(1);
        value.resize(vlen);
        in.read(value.data(), static_cast<std::streamsize>(vlen));
        in >> expire;

        std::optional<Clock::time_point> tp = std::nullopt;
        if(expire != -1){
            tp = Clock::time_point(std::chrono::seconds(expire));
        }

        lru.push_front(key);
        data[key] = {
            value,
            tp,
            lru.begin()
        };
    }
}

void KVStore::enableAOF(const std::string& filename){
    std::unique_lock<std::shared_mutex> lock(m);

    aof_filename_ = filename;
    aof_out_.open(filename, std::ios::app);
    aof_enabled_ = true;
}

void KVStore::disableAOF(){
    std::unique_lock<std::shared_mutex> lock(m);

    if(aof_out_.is_open()){
        aof_out_.close();
    }
    aof_enabled_ = false;
}

void KVStore::loadAOF(const std::string& filename) {
    // disable logging while replay
    {
        std::unique_lock<std::shared_mutex> lock(m);
        aof_enabled_ = false;
    }

    std::ifstream in(filename);
    std::string line;

    while (std::getline(in, line)) {
        std::istringstream iss(line);

        std::string cmd;
        iss >> cmd;

        if (cmd == "SET") {
            std::string k, v;
            iss >> k >> v;
            setNoLog(k, v);
        }
        else if (cmd == "SETEX") {
            std::string k, v;
            int ttl;
            iss >> k >> ttl >> v;
            setNoLog(k, v, std::chrono::seconds(ttl));
        }
        else if (cmd == "DEL") {
            std::string k;
            iss >> k;
            delNoLog(k);
        }
    }

    {
        std::unique_lock<std::shared_mutex> lock(m);
        aof_enabled_ = true;
    }
}

void KVStore::logAOF(const std::string& cmd) {
    if (!aof_enabled_) return;

    aof_out_ << cmd << "\n" << std::flush; 
}