#pragma once

#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include <string>

#include "Trie.h"

using std::cout;
using std::string;
using std::set;
using std::pair;

/*
0 - identifier
1 - keyword
2 - literal
3 - operation ([], a++/a--, ++a/--a, арифметика, отрицание, сравнение, логические операции, присваивание)
4 - punctuation ( {} () ; )
5 - ,
6 - $ (end of file)
7 - else

8 - comments

20 - string
21 - symbol
22 - number
*/

struct Lexeme {
    int type;
    string value; 
    int line;
    Lexeme() : type(0), value(""), line(0) {}
    Lexeme(int t, string v, int line) : type(t), value(v), line(line) {}
};

class LexicalAnalyzer {
public:
    LexicalAnalyzer() : trie() {}
	char* current = NULL;
	int size_text = 0;
	char* text = nullptr;
    Trie trie;
    int line_current = 0;
    set<char> set_operations_signs = { '+', '-', '[', ']', '*', '/', '%', '!', '>', '<', '=', '&', '|', '.' };
    set<string> set_operations = { "+", "-", "*", "/", "[", "]", "++", "--", "%", ">", "<", "==", ">=", "<=", "!=", "!", "&&", "||", "=", "." };
    void load() {
        std::ifstream in("program2.txt", std::ios::binary);
        in.seekg(0, std::ios::end);
        size_text = in.tellg();
        in.seekg(0);
        text = new char[size_text + 1];
        in.read(text, size_text);
        ++size_text;
        char* i = text;
        for (; i < text + size_text - 1; ++i);
        *i = '$';
        //cout << size_text << "\n";
        int type = -1;
        current = text;
        /*int ind = 0;
        for (char* i = text; i < text + size_text; ++i) {
            cout << *i << "\n";
            if (*i == '\r') {
                cout << ind << "\n";
            }
            ++ind;
        }*/

        std::ifstream in_f("Function_words.txt");
        string s;
        while (getline(in_f, s)) {
            trie.add(s);
        }
    }
    Lexeme get() {
        if (current >= text + size_text) {
            throw Lexeme(-1, "error", 0);
        }
        Lexeme p = this->get_lexeme();
        while (p.type == 8) {
            p = this->get_lexeme();
        }
        cout << p.value << "\n";
        return p;
    }
private:

    Lexeme get_lexeme() {
        while (current < text + size_text && (*current == ' ' || *current == '\n' || *current == '\r')) {
            if (*current == '\n') {
                ++line_current;
            }
            ++current;
        }
        string res = "";
        int type = -1;
        bool is_end = 0;
        char last = '?';
        int state = 0;
        bool is_string = 0;
        int start_type = -1;
        bool was_point = 0;
        bool is_comment = 0;
        if (('a' <= *current && *current <= 'z') || ('A' <= *current && *current <= 'Z')
            || *current == '_') {
            type = 0;
            state = 1;
        }
        else if (*current == '"' || *current == '\'' || (*current >= '0' && *current <= '9')) {
            type = 2;
            if (*current == '"') start_type = 0;
            else if (*current == '\'') start_type = 1;
            else start_type = 2;
        }
        else if (set_operations_signs.find(*current) != set_operations_signs.end()) {
            type = 3;
        }
        else if (*current == '(' || *current == '{' || *current == ')' || *current == '}' || *current == ';') {
            type = 4;
            is_end = 1;

        }
        else if (*current == ',') {
            type = 5;
            is_end = 1;
        }
        else if (*current == '$') {
            type = 6;
            is_end = 1;
        }
        else {
            type = 7;
            is_end = 1;
        }
        last = *current;
        res += *(current++);
        if (is_end) {
            return Lexeme( type, res, line_current );
        }
        for (; current < text + size_text && !is_end; ++current) {
            /*if (is_comment) {
                if (*current == '/') {
                    while (current < text + size_text && *current != '\n' && *current != '$') {
                        ++current;
                    }

                }
                else {
                    is_end = 1;
                    type = 7;
                    break;
                }
            }*/
            if (type == 0) {
                if (('a' <= *current && *current <= 'z') || ('A' <= *current && *current <= 'Z')
                    || *current == '_' || ('0' <= *current && *current <= '9')) {
                    res += *current;
                }
                else {
                    is_end = 1;
                    break;
                }
            }
            else if (type == 2) {
                if (start_type == 0) {
                    if (*current == '$') {
                        is_end = 1;
                        type = 7;
                        break;
                    }
                    res += *current;
                    if (*current == '"') {
                        is_end = 1;
                        ++current;
                        break;
                    }
                }
                else if (start_type == 1) {
                    if (*current == '$') {
                        is_end = 1;
                        type = 7;
                        break;
                    }
                    if (res.size() == 2) {
                        type = (*current != '\'' ? 7 : type);
                        if (*current == '\'') res += *(current++);
                        is_end = 1;
                        break;
                    }
                    else {
                        res += *current;
                    }
                }
                else {
                    if ('0' <= *current && *current <= '9') {
                        res += *current;
                    }
                    else if (*current == '.') {
                        if (was_point) {
                            is_end = 1;
                            break;
                        }
                        was_point = 1;
                        res += *current;
                    }
                    else {
                        if (last == '.') {
                            type = 7;
                        }
                        is_end = 1;
                        break;
                    }
                }
            }
            else if (type == 3) {
                if (set_operations_signs.find(*current) != set_operations_signs.end()) {
                    if (last == '/' && *current == '/') {
                        while (current < text + size_text && *current != '\n' && *current != '$') {
                            ++current;
                        }
                        type = 8;
                        res = "comment";
                        //cout << type;
                        is_end = 1;
                        break;
                    }
                    if (last == '/' && *current == '*') {
                        while (current < text + size_text && (*current != '/' || last != '*') && *current != '$') {
                            if (*current == '\n') {
                                ++line_current;
                            }
                            res += *current;
                            last = *current;
                            ++current;
                        }
                        if (last == '*' && *current == '/') {
                            type = 8;
                            res = "comment";
                            is_end = 1;
                            ++current;
                            break;
                        }
                        else {
                            type = 7;
                            is_end = 1;
                            break;
                        }
                    }
                    res += *current;
                    if (set_operations.find(res) != set_operations.end()) {
                        is_end = 1;
                        ++current;
                        break;
                    }
                    else {
                        res.pop_back();
                        if (last == '&' || last == '|') {
                            type = 7;
                            is_end = 1;
                            break;
                        }
                        else {
                            is_end = 1;
                            break;
                        }
                    }
                }
                else {
                    if (last == '&' || last == '|') {
                        type = 7;
                        is_end = 1;
                        break;
                    }
                    else {
                        is_end = 1;
                        break;
                    }
                }
            }

            /*if ((('a' <= *current && *current <= 'z') || ('A' <= *current && *current <= 'Z')
                || *current == '_') && type == 0) {
                type = 0;
            }
            else if ((*current == '"' || *current == '\'' || (*current >= '0' && *current <= '9')) && type == 2) {
                type = 2;
            }
            else if (set_operations_signs.find(*current) != set_operations_signs.end()) {
                type = 3;
            }

            else {
                type = 7;
                is_end = 1;
            }*/
            last = *current;
        }
        if (is_end) {
            if (type == 0) {
                if (trie.get(res)) {
                    type = 1;
                }
            }
            if (type == 2) {
                type = type * 10 + start_type;
            }
            return Lexeme(type, res, line_current);
        }
        return Lexeme(7, res, line_current);
    }

    
};
