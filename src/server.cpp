#include "server.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <sstream>
#include <iostream>

KVServer::KVServer(int port, size_t shards, size_t cap)
    : port_(port), store_(shards, cap) {}

void KVServer::start() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 128);

    std::cout << "Server listening on port " << port_ << "\n";

    while (true) {
        int client = accept(server_fd, nullptr, nullptr);

        std::thread(&KVServer::handleClient, this, client).detach();
    }
}

void KVServer::handleClient(int fd) {
    char buffer[1024];

    while (true) {
        int n = read(fd, buffer, sizeof(buffer)-1);
        if (n <= 0) break;

        buffer[n] = 0;

        std::istringstream iss(buffer);

        std::string cmd;
        iss >> cmd;

        std::string response;

        if (cmd == "SET") {
            std::string k, v;
            iss >> k >> v;
            store_.set(k, v);
            response = "OK\n";
        }
        else if (cmd == "SETEX") {
            std::string k, v;
            int ttl;
            iss >> k >> ttl >> v;
            store_.set(k, v, std::chrono::seconds(ttl));
            response = "OK\n";
        }
        else if (cmd == "GET") {
            std::string k;
            iss >> k;
            auto v = store_.get(k);
            response = v ? *v + "\n" : "(nil)\n";
        }
        else if (cmd == "DEL") {
            std::string k;
            iss >> k;
            store_.del(k);
            response = "OK\n";
        }
        else if (cmd == "STATS") {
            auto s = store_.stats();
            response =
                "hits=" + std::to_string(s.hits) +
                " misses=" + std::to_string(s.misses) +
                " size=" + std::to_string(s.size) + "\n";
        }
        else {
            response = "ERR unknown command\n";
        }

        write(fd, response.c_str(), response.size());
    }

    close(fd);
}
