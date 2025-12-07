#pragma once

#include <utility>
#include <algorithm>
#include <random>
#include <ctime>
#include <string>

using std::pair;
using std::make_pair;
using std::max;

template<typename Type1, typename Type2>

struct Node2 {
Node2 * left, * right;
Type1 key; Type2 value; long long priority, height;
Node2(Type1 key, Type2 val) : left(nullptr), right(nullptr), key(key), value(val), height(0) {
    std::random_device rd;
    std::mt19937 rnd(rd());
    priority = rnd();
}
};

template<typename T1, typename T2> 
class Treap {
public:
    Treap();
    Treap(Node2<T1, T2>*);
    void insert(T1 key, T2 val);
    void erase(T1 key);
private:
    Node2<T1, T2>* root;
};