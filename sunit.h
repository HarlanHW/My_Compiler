#ifndef SUNIT_H
#define SUNIT_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

typedef struct SUnit
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

#endif // SUNIT_H
