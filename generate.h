#ifndef GENERATE_H
#define GENERATE_H

#include <QString>
#include <QVector>
#include <fstream>
#include <vector>
#include <sstream>
#include "treenode.h"
#include "vtable.h"
#define TREEFILE "target2.txt"
using namespace std;
int Recursive(treeNode *Tree);
class Generate
{

public:
    Generate(treeNode *p);
    int init();
    int analyse();
    treeNode *Tree=NULL;
    friend int Recursive(treeNode *Tree);
    vector <QString> namelist;

};

#endif // GENERATE_H
