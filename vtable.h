#ifndef VTABLE_H
#define VTABLE_H
#pragma once
#include"treenode.h"
#include<stdio.h>
#include<stack>
#include<vector>
typedef enum {
    INT, VOID, ID, LP, RP, LB, RB, WHILE, IF, ELSE, RETURN, ASSIGN, OP1,
    OP2, RELOP, DEL, SEP, NUM, NL, PROGRAM, TYPE, SENBLOCK, INNERDEF, INNERVARIDEF, NEXT,
    SENSEQ, SENTENCE, ASSIGNMENT, RETURNSEN, RETBLOCK, WHILESEN, IFSEN, ELSEBOCLK, EXPRESSION,
    COMP, PLUSEX, OPPLUSDEC, TERM, OPMULDIV, FACTOR, EPSILON, END, ERROR, TEMP
}tokenTypes; //记号类型

typedef struct VNode
{
    int gId;					//全局ID，整个符号表唯一
    int lId;					//局部ID，临时变量共享一个递增的序列，
    __int64 hashValue;
    VNode *next = NULL;
    bool isTemp = false;		//是否是临时变量
}VNode;

class VTable
{
public:
    VTable() {
        allGId = allLId = 0;
        curLevel = 0;
        currLabel = 0;
        for (int i = 0; i < 256; ++i)
            vItems[i] = NULL;
    }
    ~VTable();

    //bool isValid(int id);					//查看这个gid（也就是ID对应的）是否可以调用
    int getLId(int id);
    int getGId(int id);						//通过变量的id找到对应全局Id，    -1为没有此变量
    int newTemp(treeNode *item);			//通过后缀表达式创建一个新标识符 并返回局部变量值。    （局部变量值就是t+后面的那个值）
    bool newTemp(int id);					//在符号表中注册id对应内容 针对ID类型
    void dirty(int id);						//id内容被修改，所以之前与id相关临时变量都要被删掉
    void newLevel();						//新开一行
    void backLevel();						//回退一行
    bool hasCur(int id);
    bool hasCur(treeNode * temp);
    int newLabel() {
        return currLabel++;
    }
private:
    void delNodes(int pos);					//删除链表
    int myHash(__int64 val);				//通过hash值得到唯一的全局id
    int allGId, allLId;						//符号表唯一全局ID和LocalId
    int curLevel;							//当前层数
    int currLabel;
    VNode* vItems[256];
};

#endif // VTABLE_H
