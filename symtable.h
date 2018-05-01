#ifndef SYMTABLE_H
#define SYMTABLE_H
#pragma once

#include<iostream>
#include<fstream>
#include<string.h>
#include<sstream>

using namespace std;

typedef class SUnit
{
public:
    char name[80];
    SUnit * next = nullptr;
    friend ostream &operator <<(ostream &out, SUnit &su) {
        out << su.name;
        return out;
    }
    friend ofstream &operator <<(ofstream &out, SUnit &su) {
        out << su.name;
        return out;
    }
}SUnit;

class SymTable
{
public:
    SymTable();
    int getId(char *name);
    friend ostream &operator<<(ostream &out, SymTable &st)
    {
        SUnit *temp = st.sHead->next;
        while (temp != nullptr) {
            out << *temp << endl;
            temp = temp->next;
        }
        return out;
    }
    friend ofstream &operator<<(ofstream &out, SymTable &st)
    {
        int i=0;
        SUnit *temp = st.sHead->next;
        while (temp != nullptr)
        {
            out << *temp << endl;
            temp = temp->next;
        }
        return out;
    }
    SUnit *sHead;
    SUnit *sTail;
    void clear();
};
#endif // SYMTABLE_H
