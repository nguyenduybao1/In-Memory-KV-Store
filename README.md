<p align="center">
  <h1 align="center">⚡ KV Store</h1>
  <p align="center">
    High-Performance • Sharded • In-Memory • Redis-like Key-Value Store (C++)
  </p>

  <p align="center">
    <img src="https://img.shields.io/badge/C++-17-blue.svg"/>
    <img src="https://img.shields.io/badge/CMake-3.15+-green.svg"/>
    <img src="https://img.shields.io/badge/Thread--Safe-Yes-success"/>
    <img src="https://img.shields.io/badge/Sharding-Enabled-orange"/>
    <img src="https://img.shields.io/badge/Tests-Passing-brightgreen"/>
    <img src="https://img.shields.io/badge/License-MIT-lightgrey"/>
  </p>
</p>

---

# ✨ Overview

**KV Store** is a lightweight, high-performance, in-memory key-value database
built from scratch using **modern C++17**.

It implements core ideas behind real-world systems like:

- Redis
- Memcached
- Distributed caches

Designed to practice:

- Systems programming
- Concurrency
- Networking
- Scalable backend architecture

> 💡 This project focuses on engineering fundamentals rather than frameworks.

---

# 🚀 Features

## Core
- ✅ In-memory KV storage
- ✅ O(1) GET / SET / DEL
- ✅ LRU eviction
- ✅ TTL expiration (SETEX)
- ✅ Stats monitoring

## Performance
- ✅ Sharded architecture
- ✅ Reduced lock contention
- ✅ Parallel writes
- ✅ Multi-threaded server

## Networking
- ✅ TCP socket server
- ✅ Thread-per-client model
- ✅ Simple text protocol
- ✅ Redis-style CLI testing

## Engineering
- ✅ Modular clean design
- ✅ Unit tests
- ✅ CMake build
- ✅ Zero dependencies

---

# 🧠 Architecture

## High-Level Flow

```
Clients (nc / benchmark / apps)
            │
            ▼
        TCP Server
   (thread per connection)
            │
            ▼
       ShardedKVStore
     ├── shard 0 (LRU)
     ├── shard 1 (LRU)
     ├── shard 2 (LRU)
     └── ...
```

---

## Sharding Strategy

```
shard = hash(key) % shard_count
```

### Benefits

- parallel writes
- reduced lock contention
- higher throughput
- near linear scalability

---

## Inside Each Shard

```
unordered_map   → storage
list            → LRU order
TTL metadata    → expiration
mutex           → thread safety
```

---

# ⚡ Complexity

| Operation | Time | Space |
|-----------|----------|----------|
SET | O(1) | O(1) |
GET | O(1) | O(1) |
DEL | O(1) | O(1) |
Eviction | O(1) | O(1) |

---

# 🛠 Build

## Requirements

- C++17
- CMake ≥ 3.15
- Linux / macOS / WSL

---

## Compile

```bash
git clone <your-repo>
cd kv-store

mkdir build
cd build

cmake ..
make
```

---

# ▶️ Run

```bash
./kvstore_app
```

Expected:

```
Server listening on port 6379
```

---

# 💻 CLI Usage

Open another terminal:

```bash
nc localhost 6379
```

---

## Example Session

```
SET a 10
OK

GET a
10

SETEX b 5 hello
OK

DEL a
OK

STATS
hits=1 misses=0 size=1
```

---

# 📦 Supported Commands

| Command | Description |
|-------------|----------------|
SET k v | set value |
SETEX k ttl v | set with TTL |
GET k | get value |
DEL k | delete key |
STATS | show metrics |

---

# 🧪 Testing

Run:

```bash
ctest
```

Test coverage includes:

- eviction correctness
- TTL expiration
- concurrency safety
- parallel writes
- shard correctness

---

# 📊 Benchmark (example)

Machine: i7 laptop, 8 shards

```
100k writes:   ~25ms
100k reads:    ~18ms
```

Scaling:

| Shards | Throughput |
|----------|------------|
1 | baseline |
4 | ~3.6x |
8 | ~7x |

> Results vary by hardware

---

# 🐳 Docker (optional)

## Dockerfile

Create:

```Dockerfile
FROM ubuntu:22.04

RUN apt update && apt install -y build-essential cmake

WORKDIR /app
COPY . .

RUN mkdir build && cd build && cmake .. && make

CMD ["./build/kvstore_app"]
```

---

## Build

```bash
docker build -t kvstore .
```

## Run

```bash
docker run -p 6379:6379 kvstore
```

---

# 📁 Project Structure

```
kv-store/
│
├── include/
│   ├── store.h
│   ├── sharded_store.h
│   └── server.h
│
├── src/
│   ├── store.cpp
│   ├── sharded_store.cpp
│   ├── server.cpp
│   └── main.cpp
│
├── tests/
│
├── CMakeLists.txt
└── README.md
```

---

# 🔥 Engineering Highlights

This project demonstrates:

## Systems
- TCP sockets
- manual memory control
- custom protocol

## Concurrency
- thread-per-client
- shard-based locking
- race-condition safe design

## Data Structures
- hash map
- LRU cache
- TTL expiration

## Scalability
- horizontal sharding
- reduced contention
- parallel writes

---

# 🎯 Design Goals

- fast
- simple
- predictable performance
- easy to extend
- interview-friendly architecture

---

# 🗺 Roadmap

## Short Term
- [ ] epoll / async I/O
- [ ] connection pool
- [ ] request batching

## Mid Term
- [ ] persistence (snapshot / AOF)
- [ ] replication
- [ ] HTTP API

## Long Term
- [ ] distributed cluster
- [ ] consistent hashing
- [ ] Raft consensus

---

# 📚 Learning Outcomes

After building this project you understand:

- how Redis works internally
- how to design caches
- how sharding improves scalability
- how TCP servers work
- how to write concurrent C++

Perfect practice for:

- Backend Engineering
- System Design Interviews
- Infrastructure roles
- Big Tech preparation

---

# 🤝 Contributing

```bash
fork → branch → commit → PR
```

---

# 📜 License

MIT License

---

# 👨‍💻 Author

Built with ❤️ to master:

- Systems Programming
- Backend Infrastructure
- Distributed Systems
- High-Performance C++
