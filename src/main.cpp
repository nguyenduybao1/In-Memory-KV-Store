#include <iostream>
#include <store.h>

int main(){
    KVStore store(10);

    store.set("a", "10");
    std::cout << store.get("a").value_or("(nil)") << "\n";

    store.del("a");
    std::cout << store.get("a").value_or("(nil)") << "\n";
    return 0;
}

