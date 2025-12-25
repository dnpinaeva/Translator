#pragma once

#include <utility>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <string>

using std::pair;
using std::cout;
using std::make_pair;
using std::max;

template<typename Type1, typename Type2>

struct Node2 {
    Node2<Type1, Type2>* left, * right;
    Type1 key; Type2 value; long long priority, height;
    Node2(const Node2& other) {
        key = other.key;
        value = other.value;
        priority = other.priority;
        height = other.height;
        left = nullptr;
        right = nullptr;
    }
    Node2(Type1 key, Type2 val) : left(nullptr), right(nullptr), key(key), value(val), height(0) {
        std::random_device rd;
        std::mt19937 rnd(rd());
        priority = rnd();
    }
    Node2() : key(0), value(0), left(nullptr), right(nullptr), height(0) {
        std::random_device rd;
        std::mt19937 rnd(rd());
        priority = rnd();
    }
    Node2<Type1, Type2>& operator=(const Node2<Type1, Type2>& other) {
        left = other.left;
        right = other.right;
        key = other.key;
        value = other.value;
        priority = other.priority;
        height = other.height;
        return *this;
    }
    void clear(Node2<Type1, Type2>* r) {
        if (r == nullptr) return;
        clear(r->left);
        clear(r->right);
        delete r;
    }
};

template<typename T1, typename T2> long long get_height(Node2<T1, T2>* Node2) {
    if (Node2 == nullptr) {
        return 0;
    }
    return Node2->height;
}

template<typename T1, typename T2> void update(Node2<T1, T2>* Node2) {
    Node2->height = max(get_height(Node2->left), get_height(Node2->right)) + 1;
    return;
}

template<typename T1, typename T2> pair<Node2<T1, T2>*, Node2<T1, T2>*> split_func(Node2<T1, T2>* root, int key) {
    if (root == nullptr) return make_pair(nullptr, nullptr);
    if (root->key >= key) {
        pair<Node2<T1, T2>*, Node2<T1, T2>*> res = split_func(root->left, key);
        Node2<T1, T2>* l = res.first;
        Node2<T1, T2>* r = res.second;
        root->left = r;
        update(root);
        return make_pair(l, root);
    }
    else {
        pair<Node2<T1, T2>*, Node2<T1, T2>*> res = split_func(root->right, key);
        Node2<T1, T2>* l = res.first;
        Node2<T1, T2>* r = res.second;
        root->right = l;
        update(root);
        return make_pair(root, r);
    }
}

template<typename T1, typename T2> Node2<T1, T2>* merge_func(Node2<T1, T2>* a, Node2<T1, T2>* b) {
    if (a == nullptr) return b;
    if (b == nullptr) return a;
    if (a->priority > b->priority) {
        a->right = merge_func(a->right, b);
        update(a);
        return a;
    }
    else {
        b->left = merge_func(a, b->left);
        update(b);
        return b;
    }
}

template<typename T1, typename T2> bool find_func(Node2<T1, T2>* root, T1 key) {
    if (root == nullptr) return false;
    if (root->key == key) return true;
    if (root->key > key) return find_func(root->left, key);
    return find_func(root->right, key);
}

template<typename T1, typename T2> T2 get_func(Node2<T1, T2>* root, T1 key) {
    if (root->key == key) return root->value;
    if (root->key > key) return get_func(root->left, key);
    return get_func(root->right, key);
}

template<typename T1, typename T2> Node2<T1, T2>* insert_func(Node2<T1, T2>* root, T1 key, T2 val) {
    pair<Node2<T1, T2>*, Node2<T1, T2>*> res = split_func(root, key);
    Node2<T1, T2>* l = res.first;
    Node2<T1, T2>* r = res.second;
    Node2<T1, T2>* v = new Node2<T1, T2>(key, val);
    Node2<T1, T2>* ans = merge_func(l, v);
    return merge_func(ans, r);
}

template<typename T1, typename T2> void print_func(Node2<T1, T2>* root) {
    if (root == nullptr) return;
    print_func(root->left);
    cout << "{" << root->key << ": " << root->value << "} ";
    print_func(root->right);
}

template<typename T1, typename T2> void check_func(Node2<T1, T2>* root) {
    if (root == nullptr) return;
    check_func(root->left);
    check_func(root->right);
}

template<typename T1, typename T2> Node2<T1, T2>* erase_func(Node2<T1, T2>* root, T1 key) {
    pair<Node2<T1, T2>*, Node2<T1, T2>*> res1 = split_func(root, key);
    pair<Node2<T1, T2>*, Node2<T1, T2>*> res2 = split_func(res1.second, key + 1);
    return merge_func(res1.first, res2.second);
}

template<typename T1, typename T2> Node2<T1, T2>* copy_func(Node2<T1, T2>* node) {
    if (node == nullptr) {
        return nullptr;
    }
    Node2<T1, T2>* new_node = new Node2<T1, T2>(node->key, node->value);
    new_node->priority = node->priority;
    new_node->height = node->height;
    new_node->left = copy_func(node->left);
    new_node->right = copy_func(node->right);
    return new_node;
}


template<typename T1, typename T2>
class Treap {
public:
    Treap() : root(nullptr) {};
    Treap(const Treap& other) {
        root = copy_func(other.root);
    }
    Treap(Node2<T1, T2>* tree) {
        root = tree;
    }
    void insert(T1 key, T2 val) {
        if (find_func(root, key)) {
            root = erase_func(root, key);
        }
        root = insert_func(root, key, val);
        // std::cout << root->key << " " << root->value << std::endl;
    }
    void erase(T1 key) {
        root = erase_func(root, key);
    }

    int find(T1 key) {
        if (find_func(root, key)) return 1;
        return 0;
    }
    T2 get(T1 key) {
        return get_func(root, key);
    }
    Treap& operator=(const Treap& other) {
        root = copy_func(other.root);
        return *this;
    }
    void print_treap() {
        cout << "{";
        print_func(root);
        cout << "}";
    }
    ~Treap() {
        // std::cout << "~Treap" << std::endl;
        if (root != nullptr) {
            check_func(root);
            root->clear(root);
        }
    }
    Node2<T1, T2>* root;
private:
    
};