#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <vector>
#include <algorithm>
#include <compare>
#include "domain/objects/dan_map.h"

template<typename K, typename V>
bool isSortedInOrder(const Tree<K,V>& t) {
    if (t.empty())
        return t.begin() == t.end();

    auto it = t.begin();
    auto prev = it++;
    while (it != t.end()) {
        if (!(prev->first < it->first))
            return false;
        prev = it++;
    }
    return true;
}

template<typename K, typename V>
bool iteratorRoundtripOk(const Tree<K,V>& t) {
    if (t.empty())
        return t.begin() == t.end();

    size_t cnt = 0;
    for (auto it = t.begin(); it != t.end(); ++it)
        ++cnt;

    if (cnt != t.size())
        return false;

    auto it = t.end();
    --it;
    return it != t.end();
}

TEST_CASE("iterator increment") {
    Tree<int,int> t;
    t.emplace(t.end(), 1, 10);
    t.emplace(t.end(), 2, 20);
    t.emplace(t.end(), 3, 30);

    auto it = t.begin();
    REQUIRE(it->first == 1);

    ++it;
    REQUIRE(it->first == 2);

    ++it;
    REQUIRE(it->first == 3);

    ++it;
    REQUIRE(it == t.end());

    ++it;
}

TEST_CASE("emplace duplicate") {
    Tree<int,int> t;

    auto r1 = t.emplace(t.end(), 10, 100);
    REQUIRE(r1.second);

    auto r2 = t.emplace(t.end(), 10, 200);
    REQUIRE_FALSE(r2.second);

    REQUIRE(t.size() == 1);
}

TEST_CASE("range insert") {
    std::vector<std::pair<int,int>> v;
    v.push_back(std::make_pair(3,30));
    v.push_back(std::make_pair(1,10));
    v.push_back(std::make_pair(2,20));

    Tree<int,int> t;
    t.insert(t.end(), v.begin(), v.end());

    REQUIRE(t.size() == 3);
    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("avl ll") {
    Tree<int,int> t;
    t.emplace(t.end(), 3, 0);
    t.emplace(t.end(), 2, 0);
    t.emplace(t.end(), 1, 0);

    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("avl rr") {
    Tree<int,int> t;
    t.emplace(t.end(), 1, 0);
    t.emplace(t.end(), 2, 0);
    t.emplace(t.end(), 3, 0);

    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("avl lr") {
    Tree<int,int> t;
    t.emplace(t.end(), 3, 0);
    t.emplace(t.end(), 1, 0);
    t.emplace(t.end(), 2, 0);

    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("avl rl") {
    Tree<int,int> t;
    t.emplace(t.end(), 1, 0);
    t.emplace(t.end(), 3, 0);
    t.emplace(t.end(), 2, 0);

    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("operator brackets default") {
    Tree<int,int> t;
    t[5] = 50;

    REQUIRE(t.size() == 1);
    REQUIRE(t[5] == 50);
}

TEST_CASE("at behavior") {
    Tree<int,int> t;
    REQUIRE_THROWS_AS(t.at(42), std::out_of_range);
    t.emplace(t.end(), 1, 777);
    REQUIRE(t.at(1));
}

TEST_CASE("erase basic") {
    Tree<int,int> t;
    int keys[] = {4,2,6,1,3,5,7};
    for (int k : keys)
        t.emplace(t.end(), k, k);

    REQUIRE(t.erase(1) == 1);
    REQUIRE(t.erase(2) == 1);
    REQUIRE(t.erase(4) == 1);

    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("erase right leaf") {
    Tree<int,int> t;

    t.emplace(t.end(), 10, 0);
    t.emplace(t.end(), 5,  0);
    t.emplace(t.end(), 15, 0);

    REQUIRE(t.erase(15) == 1);
    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("erase iterator range") {
    Tree<int,int> t;
    for (int i = 0; i < 5; ++i)
        t.emplace(t.end(), i, i);

    auto it = t.begin();
    ++it;
    t.erase(it);

    REQUIRE(t.size() == 4);

    t.erase(t.begin(), t.end());
    REQUIRE(t.empty());

    t.erase(t.end());
    REQUIRE(t.empty());
}

TEST_CASE("unlink left two children") {
    Tree<int,int> t;

    t.emplace(t.end(), 20, 0);
    t.emplace(t.end(), 10, 0);
    t.emplace(t.end(), 30, 0);

    t.emplace(t.end(), 5,  0);
    t.emplace(t.end(), 15, 0);

    REQUIRE(t.erase(10) == 1);
    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("unlink right two children") {
    Tree<int,int> t;

    t.emplace(t.end(), 20, 0);
    t.emplace(t.end(), 10, 0);
    t.emplace(t.end(), 30, 0);

    t.emplace(t.end(), 25, 0);
    t.emplace(t.end(), 40, 0);

    REQUIRE(t.erase(30) == 1);
    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("unlink successor child") {
    Tree<int,int> t;

    t.emplace(t.end(), 50, 0);
    t.emplace(t.end(), 80, 0);

    t.emplace(t.end(), 60, 0);
    t.emplace(t.end(), 120, 0);

    t.emplace(t.end(), 100, 0);
    t.emplace(t.end(), 90, 0);
    t.emplace(t.end(), 95, 0);

    REQUIRE(t.erase(80) == 1);
    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("unlink general left") {
    Tree<int,int> t;

    for (int k : {40,20,60,10,30,35})
        t.emplace(t.end(), k, 0);

    t.erase(20);

    auto keys = t.keys();
    REQUIRE(keys == std::vector<int>({10,30,35,40,60}));
}

TEST_CASE("unlink general right") {
    Tree<int,int> t;

    for (int k : {40,20,60,50,80,70})
        t.emplace(t.end(), k, 0);

    t.erase(60);

    auto keys = t.keys();
    REQUIRE(keys == std::vector<int>({20,40,50,70,80}));
}

TEST_CASE("copy") {
    Tree<int,int> t;
    t.emplace(t.end(), 1, 10);
    t.emplace(t.end(), 2, 20);

    Tree<int,int> c(t);
    REQUIRE(c == t);

    Tree<int,int> a;
    a = t;
    REQUIRE(a == t);
}

TEST_CASE("move swap") {
    Tree<int,int> t;
    t.emplace(t.end(), 1, 1);

    Tree<int,int> m(std::move(t));
    REQUIRE(m.size() == 1);

    Tree<int,int> x;
    x.swap(m);
    REQUIRE(x.size() == 1);
}

TEST_CASE("assign range") {
    Tree<int,int> t;

    std::vector<std::pair<int,int>> v;
    v.push_back(std::make_pair(1,1));
    v.push_back(std::make_pair(2,2));

    t.assign(v.begin(), v.end());
    REQUIRE(t.size() == 2);
}

TEST_CASE("compare equal") {
    Tree<int,int> a;
    Tree<int,int> b;

    a.emplace(a.end(), 1, 1);
    b.emplace(b.end(), 1, 1);

    REQUIRE(a == b);
    REQUIRE((a <=> b) == std::strong_ordering::equal);

    b.emplace(b.end(), 2, 2);
    REQUIRE((a <=> b) == std::strong_ordering::less);
}

TEST_CASE("clear") {
    Tree<int,int> t;
    t.emplace(t.end(), 1, 1);

    t.clear();
    REQUIRE(t.empty());
    REQUIRE(t.begin() == t.end());
}

TEST_CASE("misc") {
    Tree<int,int> t;

    REQUIRE(t.empty());
    REQUIRE(t.count(1) == 0);

    t.emplace(t.end(), 1, 10);

    REQUIRE_FALSE(t.empty());
    REQUIRE(t.count(1) == 1);
    REQUIRE(t.count(2) == 0);

    REQUIRE(t.find(1) != t.end());
    REQUIRE(t.find(2) == t.end());

    const Tree<int,int>& ct = t;
    REQUIRE(ct.find(1) != ct.end());
    REQUIRE(ct.find(2) == ct.end());

    REQUIRE(t.max_size() > 0);
}

TEST_CASE("keys") {
    Tree<int,int> t;
    t.emplace(t.end(), 3, 0);
    t.emplace(t.end(), 1, 0);
    t.emplace(t.end(), 2, 0);

    auto k = t.keys();
    REQUIRE(k.size() == 3);
    REQUIRE(k[0] == 1);
    REQUIRE(k[1] == 2);
    REQUIRE(k[2] == 3);
}

TEST_CASE("operator rvalue") {
    Tree<int,int> t;

    int x = 7;
    t[std::move(x)] = 70;

    REQUIRE(t.size() == 1);
    REQUIRE(t[7] == 70);
}

TEST_CASE("erase missing") {
    Tree<int,int> t;
    t.emplace(t.end(), 1, 1);

    REQUIRE(t.erase(42) == 0);
    REQUIRE(t.size() == 1);
}

TEST_CASE("insert n") {
    Tree<int,int> t;

    t.insert(t.end(), 3, std::make_pair(1,1));
    REQUIRE(t.size() == 1);

    t.insert(t.end(), 5, std::make_pair(1,1));
    REQUIRE(t.size() == 1);
}

TEST_CASE("insert init") {
    Tree<int,int> t;

    t.insert(t.end(), {
        {2,2},
        {1,1},
        {3,3}
    });

    REQUIRE(t.size() == 3);
    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("assign n") {
    Tree<int,int> t;

    Tree<int,int>::value_type v{1,1};
    t.assign(5, v);

    REQUIRE(t.size() == 1);
}

TEST_CASE("assign init") {
    Tree<int,int> t;

    t.assign({
        {1,1},
        {2,2},
        {3,3}
    });

    REQUIRE(t.size() == 3);
    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("iterator edges") {
    Tree<int,int> t;

    REQUIRE(t.begin() == t.end());

    t.emplace(t.end(), 1, 1);

    auto it = t.begin();
    auto it2 = it;
    ++it2;
    REQUIRE(it2 == t.end());

    --it2;
    REQUIRE(it2 == t.begin());
}

TEST_CASE("compare greater") {
    Tree<int,int> a;
    Tree<int,int> b;

    a.emplace(a.end(), 2, 2);
    b.emplace(b.end(), 1, 1);

    REQUIRE((a <=> b) == std::strong_ordering::greater);
}

TEST_CASE("iterator roundtrip") {
    Tree<int,int> t;
    for (int i = 0; i < 10; ++i)
        t.emplace(t.end(), i, i);

    REQUIRE(iteratorRoundtripOk(t));
}

TEST_CASE("iterator decrement left") {
    Tree<int,int> t;
    t.emplace(t.end(), 5, 0);
    t.emplace(t.end(), 3, 0);
    t.emplace(t.end(), 7, 0);
    t.emplace(t.end(), 4, 0);

    auto it = t.find(5);
    --it;

    REQUIRE(it->first == 4);
}

TEST_CASE("iterator decrement end") {
    Tree<int,int> t;

    t.emplace(t.end(), 10, 0);
    t.emplace(t.end(), 5,  0);
    t.emplace(t.end(), 3,  0);

    auto it = t.find(3);
    REQUIRE(it != t.end());

    --it;

    REQUIRE(it == t.end());
}

TEST_CASE("erase right one child") {
    Tree<int,int> t;
    t.emplace(t.end(), 10, 0);
    t.emplace(t.end(), 20, 0);
    t.emplace(t.end(), 30, 0);

    REQUIRE(t.erase(20) == 1);
    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("erase successor right") {
    Tree<int,int> t;
    t.emplace(t.end(), 10, 0);
    t.emplace(t.end(), 5, 0);
    t.emplace(t.end(), 20, 0);
    t.emplace(t.end(), 15, 0);
    t.emplace(t.end(), 25, 0);
    t.emplace(t.end(), 17, 0);

    REQUIRE(t.erase(10) == 1);
    REQUIRE(isSortedInOrder(t));
}

struct ThrowOnCopy {
    int v;
    ThrowOnCopy(int x) : v(x) {}
    ThrowOnCopy(const ThrowOnCopy&) { throw std::runtime_error(""); }
    ThrowOnCopy(ThrowOnCopy&&) noexcept = default;
};

TEST_CASE("copy exception") {
    Tree<int, ThrowOnCopy> t;
    t.emplace(t.end(), 1, ThrowOnCopy{1});

    REQUIRE_THROWS_AS(
        ([&]() {
            Tree<int, ThrowOnCopy> copy_(t);
        }()),
        std::runtime_error
    );

    t = t;
}

TEST_CASE("find left walk") {
    Tree<int,int> t;
    t.emplace(t.end(), 10, 0);
    t.emplace(t.end(), 5,  0);
    t.emplace(t.end(), 15, 0);

    {
        auto it = t.find(1);
        REQUIRE(it == t.end());
    }

    {
        const Tree<int,int>& ct = t;
        auto it = ct.find(1);
        REQUIRE(it == ct.end());
    }
}

TEST_CASE("operator existing") {
    Tree<int,int> t;
    t[1] = 10;
    t[1] = 20;

    REQUIRE(t[1] == 20);
}

TEST_CASE("insert all duplicates") {
    Tree<int,int> t;
    t.emplace(t.end(), 1, 1);

    std::vector<std::pair<int,int>> v{{1,1},{1,1}};
    auto it = t.insert(t.end(), v.begin(), v.end());

    REQUIRE(it == t.end());
}

TEST_CASE("iterator assignment") {
    Tree<int,int> t;
    t.emplace(t.end(), 1, 1);

    auto it = t.begin();
    auto it2 = it;
    it = it2;

    REQUIRE(it == t.begin());
}

TEST_CASE("compare mapped") {
    Tree<int,int> a;
    Tree<int,int> b;

    a.emplace(a.end(), 1, 1);
    b.emplace(b.end(), 1, 2);

    REQUIRE((a <=> b) == std::strong_ordering::less);
}

TEST_CASE("rotate left beta") {
    Tree<int,int> t;
    t.emplace(t.end(), 10, 0);
    t.emplace(t.end(), 5, 0);
    t.emplace(t.end(), 20, 0);
    t.emplace(t.end(), 15, 0);

    REQUIRE(isSortedInOrder(t));
}

TEST_CASE("find survives complex erase patterns") {
    Tree<int,int> t;


    std::vector<int> keys = {
        50, 20, 80, 10, 30, 60, 90,
        5, 15, 25, 35, 55, 70, 85, 95,
        1, 6, 14, 16, 24, 26
    };

    for (int k : keys)
        t.emplace(t.end(), k, k);

    REQUIRE(isSortedInOrder(t));


    std::vector<int> erase_order = {
        20, 80, 10, 90, 5, 15, 30, 60
    };

    for (int k : erase_order) {
        INFO("erasing " << k);
        REQUIRE(t.erase(k) == 1);
        REQUIRE(isSortedInOrder(t));


        for (int x : keys) {
            if (std::find(erase_order.begin(), erase_order.end(), x)
                < std::find(erase_order.begin(), erase_order.end(), k)) {
                REQUIRE(t.find(x) == t.end());
                } else {
                    auto it = t.find(x);
                    if (it != t.end()) {
                        REQUIRE(it->first == x);
                    }
                }
        }
    }
}

