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
    Node2 operator=(const Node2& other) {
        left = other.left;
        right = other.right;
        key = other.key;
        value = other.value;
        priority = other.priority;
        height = other.height;
        return *this;
    }
    void clear(Node2* r) {
        if (r == NULL) return;
        clear(r->left);
        clear(r->right);
        delete r;
    }
};

template<typename T1, typename T2> 
class Treap {
public:
    Treap() : root(NULL) {};
    Treap(Node2<T1, T2>*);
    void insert(T1 key, T2 val);
    void erase(T1 key);
    Treap operator=(const Treap& other) {
        root->clear(root);
        root = other.root;
        return *this;
    }
private:
    Node2<T1, T2>* root;
};