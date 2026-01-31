#include "server.h"

int main() {
    KVServer server(6379, 8, 10000);
    server.start();
};
