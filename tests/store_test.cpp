#include <gtest/gtest.h>
#include <store.h>
#include <vector>
#include <thread>

TEST(KVStoreTest, BasicSetGet){
    KVStore s;
    s.set("a", "10");

    EXPECT_EQ(s.get("a"), "10");
}

TEST(KVStoreTest, Delete){
    KVStore s;
    s.set("a", "10");
    s.del("a");

    EXPECT_EQ(s.get("a"), "");
}

TEST(KVStoreTest, ConcurrentWrite){
    KVStore s;

    std::vector<std::thread> threads;

    for(int i = 0; i < 50; i++){
        threads.emplace_back([&]{
            for(int j = 0; j < 1000; j++){
                s.set("k", "1");
            }
        });
    }

    for(auto& t : threads)
        t.join();

    
    EXPECT_EQ(s.get("k"), "1");
}