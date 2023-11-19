#define _CRT_SECURE_NO_WARNINGS 1

#include<string>
#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
using namespace std;

struct Grammar {
    set<char> Vt; // �ս��
    set<char> Vn; // ���ս��
    vector<string> Productions; // �ķ�����ʽ
    map<char, set<char>> First; // �洢���ս����First��
    map<char, set<char>> Follow; // �洢���ս����Follow��
};

bool isInSet(char c, const set<char>& s) {
    return find(s.begin(), s.end(), c) != s.end();
}//�ж��ַ�c�Ƿ��ڼ���s��

void addToSet(set<char>& A, const set<char>& B) {
    A.insert(B.begin(), B.end());
}//������B��Ԫ�ؼӵ�A��

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
                grammar.First[symbol].insert('@');//��
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

    // ��ȡ�ս��
    cout << "�������ս�����Կո�ָ�,�س���������";
    string vtInput;
    getline(cin, vtInput);
    for (char c : vtInput) {
        if (c != ' ') {
            grammar.Vt.insert(c);
        }
    }

    // ��ȡ���ս��
    cout << "��������ս�����Կո�ָ�,�س���������";
    string vnInput;
    getline(cin, vnInput);
    for (char c : vnInput) {
        if (c != ' ') {
            grammar.Vn.insert(c);
        }
    }

    // ��ȡ�ķ�����ʽ
    cout << "�������ķ�����ʽ��ÿ��һ��,�س�����,���б�ʾ�������룩��" << endl;
    string prodInput;
    while (getline(cin, prodInput) && !prodInput.empty()) {
        grammar.Productions.push_back(prodInput);
    }

    // ������ս����First��
    for (char vn : grammar.Vn) {
        calculateFirst(grammar, vn);
    }

    // ������ս����Follow��
    for (char vn : grammar.Vn) {
        calculateFollow(grammar, vn);
    }

    // ������
    int i = 1;
    cout << "���ս����First����" << endl;
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

    cout << "���ս����Follow����" << endl;
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