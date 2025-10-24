#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <utility>

using std::vector;
using std::string;

constexpr int k = 26 + 26 + 10 + 1;

struct Node {
    vector<int> next;
    bool term;
    int size;
    Node() : next(k, -1), term(false), size(0) {};
};

class Trie {
public:
    Trie(): v() {
        v.emplace_back(Node());
    }
    void add(string s) {
        int crt = 0;
        for (char c : s) {
            ++v[crt].size;
            if ('a' <= c && c <= 'z') {
                if (v[crt].next[c - 'a'] == -1) {
                    v.emplace_back();
                    v[crt].next[c - 'a'] = v.size() - 1;
                }
                crt = v[crt].next[c - 'a'];
            }
            else if ('A' <= c && c <= 'Z') {
                if (v[crt].next[c - 'A' + 26] == -1) {
                    v.emplace_back();
                    v[crt].next[c - 'A' + 26] = v.size() - 1;
                }
                crt = v[crt].next[c - 'A' + 26];
            }
            else if ('0' <= c && c <= '9') {
                if (v[crt].next[c - '0' + 52] == -1) {
                    v.emplace_back();
                    v[crt].next[c - '0' + 52] = v.size() - 1;
                }
                crt = v[crt].next[c - '0' + 52];
            }
            else {
                if (v[crt].next[c - '_' + 62] == -1) {
                    v.emplace_back();
                    v[crt].next[c - '_' + 62] = v.size() - 1;
                }
                crt = v[crt].next[c - '_' + 62];
            }
        }
        v[crt].term = true;
        ++v[crt].size;
    }
    bool get(string s) {
        int crt = 0;
        for (char c : s) {
            if ('a' <= c && c <= 'z') {
                if (v[crt].next[c - 'a'] == -1) {
                    return false;
                }
                crt = v[crt].next[c - 'a'];
            }
            else if ('A' <= c && c <= 'Z') {
                if (v[crt].next[c - 'A' + 26] == -1) {
                    return false;
                }
                crt = v[crt].next[c - 'A' + 26];
            }
            else if ('0' <= c && c <= '9') {
                if (v[crt].next[c - '0' + 52] == -1) {
                    return false;
                }
                crt = v[crt].next[c - '0' + 52];
            }
            else {
                if (v[crt].next[c - '_' + 62] == -1) {
                    return false;
                }
                crt = v[crt].next[c - '_' + 62];
            }
        }
        if (v[crt].term) {
            return true;
        }
        return false;
    }
    
private:
    vector<Node> v;
};
