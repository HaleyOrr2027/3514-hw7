// tests/test.cpp
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "array/vector.hpp"

using dsac::array::Vector;

TEST_CASE("Default constructor") {
    Vector<int> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());
    REQUIRE(v.capacity() == 0);
}

// elements are stored correctly and size grows
TEST_CASE("push_back stores elements correctly") {

    Vector<int> v;

    v.push_back(10);

    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 10);
    REQUIRE(v.front() == 10);
    REQUIRE(v.back() == 10);

    v.push_back(20);

    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
    REQUIRE(v.back() == 20);
}

// Force the vector to grow multiple times and ensure
// values remain correct after reallocation.
TEST_CASE("push_back expands capacity and preserves elements") {

    Vector<int> v;

    for (int i = 0; i < 20; ++i) {
        v.push_back(i);
    }

    REQUIRE(v.size() == 20);

    for (int i = 0; i < 20; ++i) {
        REQUIRE(v[i] == i);
    }

    REQUIRE(v.capacity() >= v.size());
}

// returns a reference so values can be modified
TEST_CASE("operator[] returns - modifiable reference") {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);

    v[0] = 99;
    REQUIRE(v[0] == 99);
    REQUIRE(v[1] == 2);
}

TEST_CASE("const operator[] returns - non-modifiable reference") {
    Vector<int> v;
    for (int i{0}; i < 5; ++i) {
        v.push_back(i);
    }

    const Vector<int>& cv = v;

    REQUIRE(cv.size() == 5);
    REQUIRE(cv[0] == 0);
    REQUIRE(cv[4] == 4);
}

//at() should throw an exception when index is invalid.
TEST_CASE("at() throws on invalid index") {

    Vector<int> v;

    v.push_back(5);
    v.push_back(6);

    REQUIRE_NOTHROW(v.at(0));
    REQUIRE_NOTHROW(v.at(1));

    REQUIRE_THROWS(v.at(-1));
    REQUIRE_THROWS(v.at(2));
}

TEST_CASE("front/back references reflect underlying data (const and non-const)") {
    Vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    REQUIRE(v.front() == 10);
    REQUIRE(v.back() == 30);

    v.front() = 111;
    v.back() = 333;

    REQUIRE(v[0] == 111);
    REQUIRE(v[2] == 333);

    const Vector<int>& cv = v;
    REQUIRE(cv.front() == 111);
    REQUIRE(cv.back() == 333);
}

TEST_CASE("reserve increases capacity and preserves existing elements") {
    Vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i);

    int oldCap = v.capacity();
    REQUIRE(oldCap >= 5);

    v.reserve(oldCap); // no change expected
    REQUIRE(v.capacity() == oldCap);
    REQUIRE(v.size() == 5);

    v.reserve(oldCap + 50);
    REQUIRE(v.capacity() >= oldCap + 50);
    REQUIRE(v.size() == 5);

    // Elements must be preserved
    for (int i = 0; i < 5; ++i) REQUIRE(v[i] == i);
}

TEST_CASE("insert shifts right and inserts correctly") {
    Vector<int> v;
    for (int i = 0; i < 5; ++i) v.push_back(i); // [0 1 2 3 4]

    SECTION("insert in middle") {
        v.insert(2, 99); // [0 1 99 2 3 4]
        REQUIRE(v.size() == 6);
        REQUIRE(v[0] == 0);
        REQUIRE(v[1] == 1);
        REQUIRE(v[2] == 99);
        REQUIRE(v[3] == 2);
        REQUIRE(v[4] == 3);
        REQUIRE(v[5] == 4);
    }

    SECTION("insert at front") {
        v.insert(0, 77); // [77 0 1 2 3 4]
        REQUIRE(v.size() == 6);
        REQUIRE(v.front() == 77);
        REQUIRE(v[1] == 0);
        REQUIRE(v.back() == 4);
    }

    SECTION("insert at end (i == sz)") {
        v.insert(v.size(), 55); // append
        REQUIRE(v.size() == 6);
        REQUIRE(v.back() == 55);
        REQUIRE(v[0] == 0);
        REQUIRE(v[4] == 4);
    }
}

TEST_CASE("insert with invalid index does nothing (your code returns)") {
    Vector<int> v;
    for (int i = 0; i < 3; ++i) v.push_back(i); // [0 1 2]
    int oldSize = v.size();
    int oldCap = v.capacity();

    v.insert(-1, 99);
    REQUIRE(v.size() == oldSize);
    REQUIRE(v.capacity() == oldCap);
    REQUIRE(v[0] == 0);

    v.insert(999, 99);
    REQUIRE(v.size() == oldSize);
    REQUIRE(v.capacity() == oldCap);
    REQUIRE(v[2] == 2);
}

TEST_CASE("erase shifts left and removes correctly (middle, front, end)") {
    Vector<int> v;
    for (int i = 0; i < 6; ++i) v.push_back(i); // [0 1 2 3 4 5]

    SECTION("erase middle") {
        v.erase(2); // remove 2 -> [0 1 3 4 5]
        REQUIRE(v.size() == 5);
        REQUIRE(v[0] == 0);
        REQUIRE(v[1] == 1);
        REQUIRE(v[2] == 3);
        REQUIRE(v[3] == 4);
        REQUIRE(v[4] == 5);
    }

    SECTION("erase front") {
        v.erase(0); // [1 2 3 4 5]
        REQUIRE(v.size() == 5);
        REQUIRE(v.front() == 1);
        REQUIRE(v[1] == 2);
    }

    SECTION("erase end (last index)") {
        v.erase(v.size() - 1); // remove 5 -> [0 1 2 3 4]
        REQUIRE(v.size() == 5);
        REQUIRE(v.back() == 4);
        REQUIRE(v[4] == 4);
    }
}

TEST_CASE("erase with invalid index does nothing (your code returns)") {
    Vector<int> v;
    for (int i = 0; i < 4; ++i) v.push_back(i);
    int oldSize = v.size();
    int oldCap = v.capacity();

    v.erase(-1);
    REQUIRE(v.size() == oldSize);
    REQUIRE(v.capacity() == oldCap);

    v.erase(4);
    REQUIRE(v.size() == oldSize);
    REQUIRE(v.capacity() == oldCap);

    for (int i = 0; i < 4; ++i) REQUIRE(v[i] == i);
}

TEST_CASE("pop_back decreases size; pop_back on empty does nothing") {
    Vector<int> v;
    v.pop_back(); // should not crash
    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    REQUIRE(v.size() == 3);
    v.pop_back();
    REQUIRE(v.size() == 2);
    REQUIRE(v.back() == 2);

    v.pop_back();
    v.pop_back();
    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());

    v.pop_back(); // still should not crash
    REQUIRE(v.size() == 0);
}

TEST_CASE("shrink() halves capacity when sz <= cap/4") {
    Vector<int> v;

    // Grow to cap 16
    for (int i = 0; i < 16; ++i) v.push_back(i);
    REQUIRE(v.size() == 16);
    REQUIRE(v.capacity() >= 16);

    REQUIRE(v.capacity() == 16);

-    for (int i = 0; i < 12; ++i) v.pop_back(); // size 4
    REQUIRE(v.size() == 4);

    // should have halved to 8
    REQUIRE(v.capacity() == 8);

    // Pop down to size 2 -> cap/4 = 2, so shrink triggers again
    v.pop_back(); // size 3
    REQUIRE(v.capacity() == 8);

    v.pop_back(); // ssize 2-
    REQUIRE(v.size() == 2);
    REQUIRE(v.capacity() == 4);
}

TEST_CASE("shrink_to_fit sets capacity to max(1, sz) and preserves elements") {
    Vector<int> v;
    for (int i = 0; i < 10; ++i) v.push_back(i);
    REQUIRE(v.capacity() >= 10);

    // remove to make sz smaller than cap
    for (int i = 0; i < 7; ++i) v.pop_back(); // size 3
    REQUIRE(v.size() == 3);

    v.shrink_to_fit();
    REQUIRE(v.capacity() == 3);
    REQUIRE(v[0] == 0);
    REQUIRE(v[1] == 1);
    REQUIRE(v[2] == 2);

    // empty case: cap becomes 1
    Vector<int> e;
    e.shrink_to_fit();
    REQUIRE(e.size() == 0);
    REQUIRE(e.capacity() == 1);
}

TEST_CASE("basic string test") {
    Vector<std::string> v;
    v.push_back("a");
    v.push_back("b");
    v.insert(1, "X");

    REQUIRE(v.size() == 3);
    REQUIRE(v.front() == "a");
    REQUIRE(v[1] == "X");
    REQUIRE(v.back() == "b");

    v.erase(0);
    REQUIRE(v.size() == 2);
    REQUIRE(v.front() == "X");
    REQUIRE(v.back() == "b");
}