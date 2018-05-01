#ifndef TREENODE_H
#define TREENODE_H
#include<stdio.h>
typedef struct treeNode
{
    int type;
    int value;
    int lineNum;
    treeNode *sonNode = NULL;
    treeNode *sibleNode = NULL;
} treeNode;

#endif // TREENODE_H
