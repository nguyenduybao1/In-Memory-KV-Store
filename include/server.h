#pragma once
#include "sharded_store.h"
#include <string>

class KVServer {
public:
    KVServer(int port, size_t shards, size_t cap_per_shard);

    void start();

private:
    void handleClient(int client_fd);

    int port_;
    ShardedKVStore store_;
};
