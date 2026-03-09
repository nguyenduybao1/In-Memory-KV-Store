#include "server.h"
#include <cstdlib>
#include <iostream>

int getEnvInt(const char* key, int def) {
    const char* val = std::getenv(key);
    return val ? std::stoi(val) : def;
}

int main() {
    int port   = getEnvInt("PORT", 6379);
    int shards = getEnvInt("SHARDS", 8);
    int cap    = getEnvInt("CAPACITY", 10000);

    std::cout
        << "Starting KVStore...\n"
        << "PORT=" << port
        << " SHARDS=" << shards
        << " CAPACITY=" << cap << "\n";

    KVServer server(port, shards, cap);
    server.start();
}
