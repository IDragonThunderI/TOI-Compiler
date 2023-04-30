#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "lexAnalyser.h"
using namespace std;

/*
1        ������ (start)
2        ���������, ��������� 1, 0 (const1)
3        ���������, ��������� 2, x (const2)
4        ������������� (id)
5        ����� �������� (+, -, *, /)
6        ���� ������������ (:=) (_equal)
7        ����������� (comment)
8        �������� �����, ��������� 1, f (for1)
9        �������� �����, ��������� 2, o (for2)
10       �������� �����, ��������� 3, r (for3)
11       ����������� ������
12       ���������� �����
13       �������� ����������� ������ ("{")
14       �������� ����������� ������ ("}")
15       �������� ������� ������ ("(")
16       �������� ������� ������ (")")
17       ������ (error)
*/

enum states
{
    start = 1,
    const1 = 2,
    const2 = 3,
    id = 4,
    _equal = 6,
    comment = 7,

    for1 = 8,
    for2 = 9,
    for3 = 10,

    error = 17,

    and1 = 18,
    and2 = 19,
    and3 = 20,

    or1 = 21,
    or2 = 22,

    not1 = 23,
    not2 = 24,
    not3 = 25,

    while1 = 26,
    while2 = 27,
    while3 = 28,
    while4 = 29,
    while5 = 30,

    do1 = 31,
    do2 = 32
};

ofstream fout("out_LA.txt", ios_base::trunc);
ofstream id_out("out_id_LA.txt", ios_base::trunc);
vector <string> id_LA;

void lexem(string s, int y, int n)
{
    bool f = true;
    fout << s;
    if (s != " ")
    {
        switch (y)
        {
        case 3:
            fout << " ��������� " << n << endl;
            break;
        case 4:
            fout << " ������������� " << n << endl;
            for (int i = 0; i < id_LA.size(); i++)
                if (s == id_LA[i])
                    f = false;
            if (f)
            {
                id_LA.push_back(s);
                id_out << s << endl;
            }
            break;
        case 5:
            fout << " ��������������_�������� " << n << endl;
            break;
        case 6:
            fout << " ����_������������ " << n << endl;
            break;
        case 7:
            fout << " ����������� " << n << endl;
            break;
        case 10: case 20: case 22: case 25: case 30: case 32:
            fout << " ��������_����� " << n << endl;
            break;
        case 11:
            fout << " �����������_������ " << n << endl;
            break;
        case 17:
            fout << " ��_�������_���������� " << n << endl;
            break;
        case 12:
            fout << " ��������_��������� " << n << endl;
            break;
        case 13:
            fout << " ��������_�����������_������ " << n << endl;
            break;
        case 14:
            fout << " ��������_�����������_������ " << n << endl;
            break;
        case 15:
            fout << " ��������_�������_������ " << n << endl;
            break;
        case 16:
            fout << " ��������_�������_������ " << n << endl;
            break;
        }
    }
}

int lexAnalysis(string file)
{
    ifstream code_in(file);

    string now_line, res = "";
    int num = 0;
    states state = start;

    while (getline(code_in, now_line))
    {
        if (res != "")
        {
            lexem(res, state, num);
            res = "";
        }

        if (state != comment)
            state = start;

        num++;

        for (int i = 0; i < now_line.size(); i++)
        {
            char n_sym = now_line[i];
            switch (state)
            {
            case (start):
                switch (n_sym)
                {
                case '0':
                    res += n_sym;
                    state = const1;
                    break;
                case '\\':
                    res += n_sym;
                    state = comment;
                    break;
                case ':':
                    res += n_sym;
                    state = _equal;
                    break;
                case '>': case '<': case '=':
                    res += n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    res += n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case 'f':
                    res += n_sym;
                    state = for1;
                    break;
                case 'a':
                    res += n_sym;
                    state = and1;
                    break;
                case 'o':
                    res += n_sym;
                    state = or1;
                    break;
                case 'n':
                    res += n_sym;
                    state = not1;
                    break;
                case 'w':
                    res += n_sym;
                    state = while1;
                    break;
                case 'd':
                    res += n_sym;
                    state = do1;
                    break;
                case 'b': case 'c':
                case 'e':
                case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
                case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v':
                case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ' ': case '\t': case '\r':
                    break;
                case ';':
                    res += n_sym;
                    lexem(res, 11, num);
                    res = "";
                    break;
                case '{':
                    res += n_sym;
                    lexem(res, 13, num);
                    res = "";
                    break;
                case '}':
                    res += n_sym;
                    lexem(res, 14, num);
                    res = "";
                    break;
                case '(':
                    res += n_sym;
                    lexem(res, 15, num);
                    res = "";
                    break;
                case ')':
                    res += n_sym;
                    lexem(res, 16, num);
                    res = "";
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (const1):
                switch (n_sym)
                {
                case 'x':
                    res += n_sym;
                    state = const2;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (const2):
                switch (n_sym)
                {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'A': case 'B': case 'C': case 'D': case 'E': case'F':
                    res += n_sym;
                    state = const2;
                    break;
                case ':':
                    lexem(res, 3, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 3, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '\\':
                    lexem(res, 3, num);
                    res = n_sym;
                    state = comment;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 3, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (id):
                switch (n_sym)
                {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (_equal):
                switch (n_sym)
                {
                case '=':
                    res += n_sym;
                    lexem(res, 6, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (comment):
                switch (n_sym)
                {
                case '\\':
                    res += n_sym;
                    lexem(res, 7, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = comment;
                    break;
                }
                break;
            case (for1):
                switch (n_sym)
                {
                case 'o':
                    res += n_sym;
                    state = for2;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
                case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (for2):
                switch (n_sym)
                {
                case 'r':
                    res += n_sym;
                    state = for3;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q':
                case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (for3):
                switch (n_sym)
                {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 10, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 10, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 10, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 10, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (and1):
                switch (n_sym)
                {
                case 'n':
                    res += n_sym;
                    state = and2;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
                case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    break;
                }
                break;
            case (and2):
                switch (n_sym)
                {
                case 'd':
                    res += n_sym;
                    state = and3;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c':
                case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    break;
                }
                break;
            case (and3):
                switch (n_sym)
                {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 20, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 20, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 20, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 20, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (or1):
                switch (n_sym)
                {
                case 'r':
                    res += n_sym;
                    state = or2;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q':
                case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (or2):
                switch (n_sym)
                {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 22, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 22, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 22, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 22, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (not1):
                switch (n_sym)
                {
                case 'o':
                    res += n_sym;
                    state = not2;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
                case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (not2):
                switch (n_sym)
                {
                case 't':
                    res += n_sym;
                    state = not3;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's':
                case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (not3):
                switch (n_sym)
                {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 25, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 25, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 25, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 25, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (while1):
                switch (n_sym)
                {
                case 'h':
                    res += n_sym;
                    state = while2;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
                case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (while2):
                switch (n_sym)
                {
                case 'i':
                    res += n_sym;
                    state = while3;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
                case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (while3):
                switch (n_sym)
                {
                case 'l':
                    res += n_sym;
                    state = while4;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k':
                case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (while4):
                switch (n_sym)
                {
                case 'e':
                    res += n_sym;
                    state = while5;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd':
                case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (while5):
                switch (n_sym)
                {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 30, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 30, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 30, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 30, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 30, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 30, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 30, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 30, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 30, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 30, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (do1):
                switch (n_sym)
                {
                case 'o':
                    res += n_sym;
                    state = do2;
                    break;
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
                case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 4, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 4, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 4, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (do2):
                switch (n_sym)
                {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
                case '_':
                    res += n_sym;
                    state = id;
                    break;
                case ':':
                    lexem(res, 32, num);
                    res = n_sym;
                    state = _equal;
                    break;
                case '\\':
                    lexem(res, 32, num);
                    res = n_sym;
                    state = comment;
                    break;
                case ' ': case '\t': case '\r':
                    lexem(res, 32, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 32, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    res = "";
                    state = start;
                    break;
                case '+': case '-': case '*': case '/':
                    lexem(res, 32, num);
                    res = n_sym;
                    lexem(res, 5, num);
                    res = "";
                    state = start;
                    break;
                case '>': case '<': case '=':
                    lexem(res, 32, num);
                    res = n_sym;
                    lexem(res, 12, num);
                    res = "";
                    state = start;
                    break;
                case '{':
                    lexem(res, 32, num);
                    res = n_sym;
                    lexem(res, 13, num);
                    res = "";
                    state = start;
                    break;
                case '}':
                    lexem(res, 32, num);
                    res = n_sym;
                    lexem(res, 14, num);
                    res = "";
                    state = start;
                    break;
                case '(':
                    lexem(res, 32, num);
                    res = n_sym;
                    lexem(res, 15, num);
                    res = "";
                    state = start;
                    break;
                case ')':
                    lexem(res, 32, num);
                    res = n_sym;
                    lexem(res, 16, num);
                    res = "";
                    state = start;
                    break;
                default:
                    res += n_sym;
                    state = error;
                    break;
                }
                break;
            case (error):
                switch (n_sym)
                {
                case ' ': case '\t': case '\r':
                    lexem(res, 17, num);
                    res = "";
                    state = start;
                    break;
                case ';':
                    lexem(res, 17, num);
                    res = n_sym;
                    lexem(res, 11, num);
                    state = start;
                    break;

                default:
                    res += n_sym;
                    break;
                }
                break;
            default:
                break;
            }
        }
    }

    if (res != "")
    {
        lexem(res, state, num);
        res = "";
    }

    if (state != comment)
        state = start;

    code_in.close();
    fout.close();
    id_out.close();

    return 0;
}