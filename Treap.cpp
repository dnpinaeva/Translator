#include "Treap.h"

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
    if (root->value >= key) {
        pair<Node2*, Node2*> res = split_func(root->left, key);
        Node2* l = res.first;
        Node2* r = res.second;
        root->left = r;
        update(root);
        return make_pair(l, root);
    }
    else {
        pair<Node2*, Node2*> res = split_func(root->right, key);
        Node2* l = res.first;
        Node2* r = res.second;
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
    if (root->value == key) return true;
    if (root->value > key) return find_func(root->left, key);
    return find_func(root->right, key);
}

template<typename T1, typename T2> Node2<T1, T2>* insert_func(Node2<T1, T2>* root, T1 key, T2 val) {
    if (find_func(root, key)) return root;
    pair<Node2*, Node2*> res = split_func(root, key);
    Node2* l = res.first;
    Node2* r = res.second;
    Node2* v = new Node2(key, val);
    Node2* ans = merge_func(l, v);
    return merge_func(ans, r);
}

template<typename T1, typename T2> Node2<T1, T2>* erase_func(Node2<T1, T2>* root, T1 key) {
    pair<Node2*, Node2*> res1 = split_func(root, key);
    pair<Node2*, Node2*> res2 = split_func(res1.second, key + 1);
    return merge_func(res1.first, res2.second);
}

template<typename T1, typename T2> Treap<T1, T2>::Treap(Node2<T1, T2>* tree) {
    root = tree;
}

template<typename T1, typename T2> void Treap<T1, T2>::insert(T1 key, T2 val) {
    root = insert_func(root, key, val);
}

template<typename T1, typename T2> void Treap<T1, T2>::erase(T1 key) {
    root = erase_func(root, key);
}

template<typename T1, typename T2> int Treap<T1, T2>::find(T1 key) {
    if (find_func(root, key)) return 1;
    return 0;
}
