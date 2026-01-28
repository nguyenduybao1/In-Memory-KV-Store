#include <gtest/gtest.h>
#include <store.h>
#include <vector>
#include <thread>
#include <chrono>

TEST(KVStoreTest, BasicSetGet){
    KVStore s(10);
    s.set("a", "1");

    auto v = s.get("a");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, "1");
    EXPECT_FALSE(s.get("missing").has_value());
}


TEST(KVStoreTest, Delete){
    KVStore s(10);
    s.set("a", "1");
    s.del("a");

    EXPECT_FALSE(s.get("a").has_value());
}

TEST(KVStoreTest, ConcurrentWrite){
    KVStore s(10);

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
    KVStore s(10);

    s.set("a", "1", std::chrono::seconds(1));

    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_FALSE(s.get("a").has_value());
}

TEST(KVStoreTest, Eviction){
    KVStore s(3);

    s.set("a", "1");
    s.set("b", "2");
    s.set("c", "3");
    s.set("d", "4");

    EXPECT_FALSE(s.get("a").has_value());
}

TEST(KVStoreTest, GetMakesKeyMostRecent){
    KVStore s(2);

    s.set("a", "1");
    s.set("b", "2");

    s.get("a");      
    s.set("c", "3");    

    EXPECT_TRUE(s.get("a").has_value());
    EXPECT_FALSE(s.get("b").has_value());
}

TEST(KVStoreTest, Metrics){
    KVStore s(2);

    s.set("a","1");

    s.get("a"); 
    s.get("a");
    s.get("x");

    s.set("b","2");
    s.set("c","3");

    auto st = s.stats();

    EXPECT_EQ(st.hits, 2);
    EXPECT_EQ(st.misses, 1);
    EXPECT_EQ(st.evictions, 1);
    EXPECT_EQ(st.size, 2);
}

TEST(KVStoreTest, RDBSaveLoadBasic){
    {
        KVStore s(10);
    
        s.set("a", "1");
        s.set("b", "2");
    
        s.save("dump.rdb");  
    }

    {
        KVStore s2(10);
        s2.load("dump.rdb");

        EXPECT_EQ(s2.get("a"), "1");
        EXPECT_EQ(s2.get("b"), "2");
    }
}


TEST(KVStoreTest, RDBWithTTL) {
    KVStore s(10);

    s.set("a", "1", std::chrono::seconds(1));
    s.save("dump.rdb");

    KVStore s2(10);
    s2.load("dump.rdb");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    EXPECT_FALSE(s2.get("a").has_value());
}


TEST(KVStoreTest, AOFReplay) {
    KVStore s(10);

    s.enableAOF("log.aof");

    s.set("x", "10");
    s.set("y", "20");
    s.del("x");

    s.disableAOF();

    KVStore s2(10);
    s2.loadAOF("log.aof");

    EXPECT_FALSE(s2.get("x").has_value());
    EXPECT_EQ(s2.get("y"), "20");
}

TEST(KVStoreTest, CrashRecoveryAOF) {
    {
        KVStore s(10);
        s.enableAOF("crash.aof");
        s.set("a", "1");
        s.set("b", "2");
    }

    KVStore s2(10);
    s2.loadAOF("crash.aof");

    EXPECT_EQ(s2.get("a"), "1");
    EXPECT_EQ(s2.get("b"), "2");
}
