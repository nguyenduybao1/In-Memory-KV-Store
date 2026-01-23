#include "store.h"

void KVStore::set(const std::string& key, const std::string& value){
    data[key] = value;
}

std::string KVStore::get(const std::string& key){
   if(data.count(key)){
        return data[key];
   }
   return "";
}

void KVStore::del(const std::string& key){
    data.erase(key);
}
