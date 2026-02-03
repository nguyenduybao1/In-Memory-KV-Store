<p align="center">
  <h1 align="center">⚡ KV Store</h1>


  <p align="center">
    <img src="https://img.shields.io/badge/language-C++17-blue.svg"/>
    <img src="https://img.shields.io/badge/build-CMake-green.svg"/>
    <img src="https://img.shields.io/badge/tests-GTest-orange.svg"/>
    <img src="https://img.shields.io/badge/docker-ready-2496ED?logo=docker&logoColor=white"/>
    <img src="https://img.shields.io/badge/thread--safe-yes-success"/>
    <img src="https://img.shields.io/badge/persistence-RDB%20%2B%20AOF-purple"/>
    <img src="https://img.shields.io/badge/license-MIT-lightgrey"/>
  </p>

  <p align="center">
    <b>Production-style in-memory database built from scratch</b><br/>
      LRU • TTL • Snapshot • AOF • Sharding • TCP Server • Docker
  </p>
</p>





---

## 🚀 Overview

**KVStore** is a Redis-inspired key-value database implemented entirely in **C++17**.

Built to deeply understand:

- Cache design
- Concurrency
- Persistence
- Sharding
- Network programming
- Systems engineering
- Docker deployment

Think of it as:

> 🧠 **Mini Redis built from scratch**

---

## ✨ Features

### 🧩 Core Storage
- O(1) HashMap operations
- LRU eviction policy
- TTL expiration
- Thread-safe (shared_mutex)
- Runtime metrics (hits / misses / evictions)

### 💾 Persistence
- RDB snapshot (save/load)
- AOF append-only log
- Crash recovery
- Durable replay

### ⚡ Scaling
- Sharding / partitioning
- Multiple KVStore instances
- Per-shard locking
- Parallel writes

### 🌐 Networking
- TCP server
- Multi-client support (thread per connection)
- Simple Redis-like text protocol

### 🐳 DevOps
- Dockerized
- Docker Compose ready
- GoogleTest suite
- CMake build

---

## 🧠 Architecture

```
Client (nc / app)
        ↓ TCP
     KVServer
        ↓
  ShardedKVStore
        ↓
+-----------+  +-----------+  +-----------+
| KVStore 0 |  | KVStore 1 |  | KVStore N |
+-----------+  +-----------+  +-----------+
      ↓             ↓             ↓
 HashMap + LRU + TTL + Persistence
```

### Design Goals

- O(1) operations
- Fine-grained locks
- Horizontal scalability
- Durable storage
- Clean modular components

---

## ⚙️ Build

### Requirements

- C++17
- CMake ≥ 3.15
- Linux / macOS / WSL

### Local build
```bash
git clone https://github.com/nguyenduybao1/In-Memory-KV-Store
cd kv-store

mkdir build
cd build

cmake ..
make
```

---

## ▶️ Run

```bash
./kvstore_app
```

Expected:

```
Server listening on port 6379
```

---

## 🖥️ Connect Client

Open another terminal:

```bash
nc localhost 6379
```

Example:

```
SET a 10
-> OK

GET a
-> 10

SETEX b 5 hello
-> OK

DEL a
-> OK

STATS
-> hits=1 misses=0 size=1
```

## ▶️ Run Test

```bash
ctest
```

---

# 📦 Supported Commands

| Command | Description | Example |
|-------------|----------------|----------------|
SET k v | set value | SET a 10
SETEX k ttl v | set with TTL | SETEX b 5 hello |
GET k | get value | GET a |
DEL k | delete key | DEL a |
STATS | show metrics | STATS |

---

## 🐳 Docker

### Build image
```bash
docker build -t kvstore .
```

### Run container
```bash
docker run -p 6379:6379 kvstore
```

Then connect:

```bash
nc localhost 6379
```

---

## 🐳 Docker Compose (recommended)

```bash
docker compose up --build
```

---

## 🧪 Test Coverage

✔ LRU eviction  
✔ TTL expiration  
✔ Concurrent writes  
✔ Metrics correctness  
✔ RDB snapshot  
✔ AOF replay  
✔ Crash recovery  
✔ Sharded parallel writes  

---

## 📂 Project Structure

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
│   └── store_test.cpp
│
├── Dockerfile
├── docker-compose.yml
├── CMakeLists.txt
└── README.md
```

---

## 📊 Complexity

| Operation | Time | Space |
|-----------|---------|-----------|
| GET | O(1) | O(1) |
| SET | O(1) | O(1) |
| DEL | O(1) | O(1) |
| LRU | O(1) | O(n) |
| Sharded write | O(1) amortized | O(n) |

---

## 🛠 Tech Stack

- C++17
- STL containers
- shared_mutex
- GoogleTest
- CMake
- Linux sockets
- Docker

---

## 🔥 What This Project Demonstrates

This project showcases:

- Data Structures and Algorithms
- Systems Design
- Multithreading & locking strategies
- Cache algorithms (LRU)
- Persistence (snapshot + WAL)
- Sharding & partitioning
- TCP networking
- Containerization
- Production-style engineering

👉 Strong preparation for **Backend / Systems / Big Tech interviews**

---

## 👤 Author

**Bảo Nguyễn**

Learning journey toward:
**Data Structures and Algorithms • Backend • Distributed Systems • Big Tech Engineering**

---

## ⭐ Support

If you find this project helpful:

⭐ Star the repo  
🍴 Fork it  
🚀 Build something cool on top  

---

