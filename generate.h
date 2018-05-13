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
    vector <string> namelist;
    int laberlist[3];
    Generate(treeNode *p);
    int init();
    int analyse();
    treeNode *Tree=NULL;
    int Recursive(treeNode *Tree);
    int expresion(treeNode *p,int id);
    int _indef(treeNode *p);
    int _assign(treeNode *p);
    int _else(treeNode *p);
    int _while(treeNode *p);
    int _if(treeNode *p);

};

#endif // GENERATE_H
