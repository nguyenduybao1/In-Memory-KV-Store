#include <iostream>
#include "store.h"

int main(){
    KVStore store;

    store.set("a", "10");
    std::cout << store.get("a") << "\n";

    store.del("a");
    std::cout << store.get("a") << "\n";
    return 0;
}