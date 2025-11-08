// LexicalAnalyzer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "LexicalAnalyzer.h"

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
0 - идентификатор
1 - служебное слово
2 - литерал
3 - операция ([], a++/a--, ++a/--a, арифметика, отрицание, сравнение, логические операции, присваивание)
4 - пунктуация ( {} () ; )
5 - ,
6 - $ (конец файла)
7 - else

6 - комменты :( :)
*/

//char* current = NULL;
//int size_text;
//char* text;
//
//set<char> set_operations_signs = { '+', '-', '[', ']', '*', '/', '%', '!', '>', '<', '=', '&', '|', '.'};
//set<string> set_operations = { "+", "-", "*", "/", "[", "]", "++", "--", "%", ">", "<", "==", ">=", "<=", "!=", "!", "&&", "||", "=", "."};
//
//struct Lexeme {
//    int type;
//    string value;
//    Lexeme() : type(0), value("") {}
//    Lexeme(int t, string v) : type(t), value(v) {}
//};
//
//Lexeme LexicalAnalyzer(Trie& trie) {
//    while (current < text + size_text && (*current == ' ' || *current == '\n' || *current == '\r')) ++current;
//    string res = "";
//    int type = -1;
//    bool is_end = 0;
//    char last = '?';
//    int state = 0;
//    bool is_string = 0;
//    int start_type = -1;
//    bool was_point = 0;
//    bool is_comment = 0;
//    if (('a' <= *current && *current <= 'z') || ('A' <= *current && *current <= 'Z')
//        || *current == '_') {
//        type = 0;
//        state = 1;
//    }
//    else if (*current == '"' || *current == '\'' || (*current >= '0' && *current <= '9')) {
//        type = 2;
//        if (*current == '"') start_type = 0;
//        else if (*current == '\'') start_type = 1;
//        else start_type = 2;
//    }
//    else if (set_operations_signs.find(*current) != set_operations_signs.end()) {
//        type = 3;
//    }
//    else if (*current == '(' || *current == '{' || *current == ')' || *current == '}' || *current == ';') {
//        type = 4;
//        is_end = 1;
//
//    }
//    else if (*current == ',') {
//        type = 5;
//        is_end = 1;
//    }
//    else if (*current == '$') {
//        type = 6;
//        is_end = 1;
//    }
//    else {
//        type = 7;
//        is_end = 1;
//    }
//    last = *current;
//    res += *(current++);
//    if (is_end) {
//        return { type, res };
//    }
//    for (; current < text + size_text && !is_end; ++current) {
//        /*if (is_comment) {
//            if (*current == '/') {
//                while (current < text + size_text && *current != '\n' && *current != '$') {
//                    ++current;
//                }
//                
//            }
//            else {
//                is_end = 1;
//                type = 7;
//                break;
//            }
//        }*/
//        if (type == 0) {
//            if (('a' <= *current && *current <= 'z') || ('A' <= *current && *current <= 'Z')
//                || *current == '_' || ('0' <= *current && *current <= '9')) {
//                res += *current;
//            }
//            else {
//                is_end = 1;
//                break;
//            }
//        } else if (type == 2) {
//            if (start_type == 0) {
//                if (*current == '$') {
//                    is_end = 1;
//                    type = 7;
//                    break;
//                }
//                res += *current;
//                if (*current == '"') {
//                    is_end = 1;
//                    ++current;
//                    break;
//                }
//            }
//            else if (start_type == 1) {
//                if (*current == '$') {
//                    is_end = 1;
//                    type = 7;
//                    break;
//                }
//                if (res.size() == 2) {
//                    type = (*current != '\'' ? 7 : type);
//                    if (*current == '\'') res += *(current++);
//                    is_end = 1;
//                    break;
//                }
//                else {
//                    res += *current;
//                }
//            }
//            else {
//                // ЧИСЕЛКА РАЗОБРАТЬ НАДО
//                if ('0' <= *current && *current <= '9') {
//                    res += *current;
//                }
//                else if (*current == '.') {
//                    if (was_point) {
//                        is_end = 1;
//                        break;
//                    }
//                    was_point = 1;
//                    res += *current;
//                }
//                else {
//                    if (last == '.') {
//                        type = 7;
//                    }
//                    is_end = 1;
//                    break;
//                }
//            }
//        }
//        else if (type == 3) {
//            if (set_operations_signs.find(*current) != set_operations_signs.end()) {
//                if (last == '/' && *current == '/') {
//                    while (current < text + size_text && *current != '\n' && *current != '$') {
//                        ++current;
//                    }
//                    type = 8;
//                    res = "comment";
//                    //cout << type;
//                    is_end = 1;
//                    break;
//                }
//                if (last == '/' && *current == '*') {
//                    while (current < text + size_text && (*current != '/' || last != '*') && *current != '$') {
//                        res += *current;
//                        last = *current;
//                        ++current;
//                    }
//                    if (last == '*' && *current == '/') {
//                        type = 8;
//                        res = "comment";
//                        is_end = 1;
//                        ++current;
//                        break;
//                    }
//                    else {
//                        type = 7;
//                        is_end = 1;
//                        break;
//                    }
//                }
//                res += *current;
//                if (set_operations.find(res) != set_operations.end()) {
//                    is_end = 1;
//                    ++current;
//                    break;
//                }
//                else {
//                    res.pop_back();
//                    if (last == '&' || last == '|') {
//                        type = 7;
//                        is_end = 1;
//                        break;
//                    }
//                    else {
//                        is_end = 1;
//                        break;
//                    }
//                }
//            }
//            else {
//                if (last == '&' || last == '|') {
//                    type = 7;
//                    is_end = 1;
//                    break;
//                }
//                else {
//                    is_end = 1;
//                    break;
//                }
//            }
//        }
//
//        /*if ((('a' <= *current && *current <= 'z') || ('A' <= *current && *current <= 'Z')
//            || *current == '_') && type == 0) {
//            type = 0;
//        }
//        else if ((*current == '"' || *current == '\'' || (*current >= '0' && *current <= '9')) && type == 2) {
//            type = 2;
//        }
//        else if (set_operations_signs.find(*current) != set_operations_signs.end()) {
//            type = 3;
//        }
//        
//        else {
//            type = 7;
//            is_end = 1;
//        }*/
//        last = *current;
//    }
//    if (is_end) {
//        if (type == 0) {
//            if (trie.get(res)) {
//                type = 1;
//            }
//        }
//        return Lexeme( type, res );
//    }
//    return Lexeme( 7, res );
//}

int main()
{
    //std::ifstream in("program2.txt", std::ios::binary);
    //in.seekg(0, std::ios::end);
    //size_text = in.tellg();
    //in.seekg(0);
    //text = new char[size_text + 1];
    //in.read(text, size_text);
    //++size_text;
    //char* i = text;
    //for (; i < text + size_text - 1; ++i);
    //*i = '$';
    //cout << size_text << "\n";
    //int type = -1;
    //current = text;
    ///*int ind = 0;
    //for (char* i = text; i < text + size_text; ++i) {
    //    cout << *i << "\n";
    //    if (*i == '\r') {
    //        cout << ind << "\n";
    //    }
    //    ++ind;
    //}*/

    //Trie trie;
    //std::ifstream in_f("Function_words.txt");
    //string s;
    //while (getline(in_f, s)) {
    //    trie.add(s);
    //}
    //cout << trie.v.size() << "\n";
    LexicalAnalyzer lexer;
    lexer.load();
    //cout << lexer.trie.v.size() << "\n";

    while (lexer.current < lexer.text + lexer.size_text) {
        Lexeme p = lexer.get();
        cout << p.type << " " << p.value << "\n";
    }

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
