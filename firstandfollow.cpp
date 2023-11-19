#define _CRT_SECURE_NO_WARNINGS 1

#include<string>
#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
using namespace std;

struct Grammar {
    set<char> Vt; // 终结符
    set<char> Vn; // 非终结符
    vector<string> Productions; // 文法产生式
    map<char, set<char>> First; // 存储非终结符的First集
    map<char, set<char>> Follow; // 存储非终结符的Follow集
};

bool isInSet(char c, const set<char>& s) {
    return find(s.begin(), s.end(), c) != s.end();
}//判断字符c是否在集合s中

void addToSet(set<char>& A, const set<char>& B) {
    A.insert(B.begin(), B.end());
}//将集合B的元素加到A中

void calculateFirst(Grammar& grammar, char symbol) {
    for (const string& prod : grammar.Productions) {
        if (prod[0] == symbol) {
            if (isInSet(prod[3], grammar.Vt)) {
                grammar.First[symbol].insert(prod[3]);
            }
            else if (isInSet(prod[3], grammar.Vn)) {
                calculateFirst(grammar, prod[3]);
                addToSet(grammar.First[symbol], grammar.First[prod[3]]);
            }
            else if (prod[3] == '@') {
                grammar.First[symbol].insert('@');//空
            }
        }
    }
}

void calculateFollow(Grammar& grammar, char symbol) {
    for (const string& prod : grammar.Productions) {
        size_t pos = prod.find(symbol);
        while (pos != string::npos) {
            if (pos + 1 < prod.length()) {
                char nextSymbol = prod[pos + 1];
                if (isInSet(nextSymbol, grammar.Vt)) {
                    grammar.Follow[symbol].insert(nextSymbol);
                }
                else if (isInSet(nextSymbol, grammar.Vn)) {
                    addToSet(grammar.Follow[symbol], grammar.First[nextSymbol]);
                    if (isInSet('@', grammar.First[nextSymbol])) {
                        calculateFollow(grammar, nextSymbol);
                        addToSet(grammar.Follow[symbol], grammar.Follow[nextSymbol]);
                    }
                }
            }
            pos = prod.find(symbol, pos + 1);
        }
    }
}

int main() {
    Grammar grammar;

    // 获取终结符
    cout << "请输入终结符（以空格分隔,回车结束）：";
    string vtInput;
    getline(cin, vtInput);
    for (char c : vtInput) {
        if (c != ' ') {
            grammar.Vt.insert(c);
        }
    }

    // 获取非终结符
    cout << "请输入非终结符（以空格分隔,回车结束）：";
    string vnInput;
    getline(cin, vnInput);
    for (char c : vnInput) {
        if (c != ' ') {
            grammar.Vn.insert(c);
        }
    }

    // 获取文法产生式
    cout << "请输入文法产生式（每行一个,回车结束,空行表示结束输入）：" << endl;
    string prodInput;
    while (getline(cin, prodInput) && !prodInput.empty()) {
        grammar.Productions.push_back(prodInput);
    }

    // 计算非终结符的First集
    for (char vn : grammar.Vn) {
        calculateFirst(grammar, vn);
    }

    // 计算非终结符的Follow集
    for (char vn : grammar.Vn) {
        calculateFollow(grammar, vn);
    }

    // 输出结果
    int i = 1;
    cout << "非终结符的First集：" << endl;
    for (char vn : grammar.Vn) {
        cout << "First(" << vn << ")={";
        i = 1;
        for (char f : grammar.First[vn]) {
            
            if (i == grammar.First[vn].size()) cout << f;
            else cout << f << ",";
            i++;
        }
        cout << "}" << endl;
    }

    cout << "非终结符的Follow集：" << endl;
    for (char vn : grammar.Vn) {
        cout << "Follow(" << vn << ")={";
        i = 1;
        for (char f : grammar.Follow[vn]) {
            
            if (i == grammar.Follow[vn].size()) cout << f;
            else cout << f << ",";
            i++;
        }
        cout << "}" << endl;
    }

    return 0;
}