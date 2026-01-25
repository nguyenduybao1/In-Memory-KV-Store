#include <gtest/gtest.h>
#include <store.h>
#include <vector>
#include <thread>

TEST(KVStoreTest, BasicSetGet){
    KVStore s;
    s.set("a", "10");

    auto v = s.get("a");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, "10");
    EXPECT_FALSE(s.get("missing").has_value());
}

TEST(KVStoreTest, Delete){
    KVStore s;
    s.set("a", "10");
    s.del("a");

    EXPECT_FALSE(s.get("a").has_value());
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

    
    auto v = s.get("k");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, "1");
}

TEST(KVStoreTest, TTLExpiration){
    KVStore s;

    s.set("a", "10", std::chrono::seconds(1));

    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_FALSE(s.get("a").has_value());
}