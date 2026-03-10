#include "server.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <sstream>
#include <iostream>
#include <stdexcept>

KVServer::KVServer(int port, size_t shards, size_t cap)
    : port_(port), store_(shards, cap) {}

void KVServer::start() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0){
        throw std::runtime_error("socket() failed");
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port_);

    if(bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0){
        close(server_fd);
        throw std::runtime_error("bind() failed on port " + std::to_string(port_));
    }
    
    if(listen(server_fd, 128) < 0){
        close(server_fd);
        throw std::runtime_error("listen() failed");
    }

    std::cout << "Server listening on port " << port_ << "\n";

    while (true) {
        int client = accept(server_fd, nullptr, nullptr);
        if(client < 0){
            std::cerr << "accept() failed, continuing...\n";
            continue;
        }
        std::thread(&KVServer::handleClient, this, client).detach();
    }
}

void KVServer::handleClient(int fd) {
    std::string recvBuf;
    char tmp[4096];

    while (true) {
        int n = read(fd, tmp, sizeof(tmp));
        if (n <= 0) break;

        recvBuf.append(tmp, n);

        std::string::size_type pos;
        while((pos = recvBuf.find('\n')) != std::string::npos){
            std::string line = recvBuf.substr(0, pos);
            recvBuf.erase(0, pos + 1);

            if(!line.empty() && line.back() == '\r'){
                line.pop_back();
            }
            if(line.empty()) continue;

            std::istringstream iss(line);
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
                    "hits="       + std::to_string(s.hits)      +
                    " misses="    + std::to_string(s.misses)     +
                    " evictions=" + std::to_string(s.evictions)  +
                    " size="      + std::to_string(s.size)       + "\n";
            }
            else {
                response = "ERR unknown command\n";
            }

            write(fd, response.c_str(), response.size());
        }
    }
    close(fd);
}
