#include "generate.h"
#include <fstream>
#include <vector>
#include<sstream>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include "treenode.h"
#include <iostream>
#include <QStack>
#include <string>
#include "vtable.h"
VTable gtable;
#define OUTFILE "generate.txt"
ofstream out_g;
using namespace std;
int tempNum=5;
int laberNum=0;
int flag=1;
QString strTokens[50] =
{ "INT","VOID","ID","LP","RP",
  "LB", "RB", "WHILE", "IF", "ELSE",
  "RETURN", "ASSIGN", "OP1", "OP2", "RELOP",
  "DEL", "SEP", "NUM", "NL", "PROGRAM",
  "TYPE", "SENBLOCK", "INNERDEF", "INNERVARIDEF", "NEXT",
  "SENSEQ", "SENTENCE", "ASSIGNMENT", "RETURNSEN", "RETBLOCK",
  "WHILESEN", "IFSEN", "ELSEBOCLK", "EXPRESSION", "COMP",
  "PLUSEX", "OPPLUSDEC", "TERM", "OPMULDIV", "FACTOR",
  "EPSILON", "END", "ERROR","TEMP" };
/*************************************************************
 *写个注释
 *INT（VOID) ：声明int（void）类型变量.
 *SENBLOCK:语句块
 *INNERDEF：内部声明.INNERVARIDEF:内部变量声明
 *后面我看懂了就不写啦  =^=!!!
 * ***********************************************************/

Generate::Generate(treeNode *p)
{
    Tree=p;
    QFile symbol("symbol.txt");
    if(!symbol.open(QFile::ReadOnly|QFile::Text))
           qDebug()<<"error";
    QTextStream file_s(&symbol);
    while(!file_s.atEnd())
    {
        QString s=file_s.readLine();
        namelist.push_back(s);
        //qDebug()<<s;
    }
}

int Generate::init()
{
    ;
}
int Generate::analyse()
{
   out_g.open(OUTFILE,ios::out);
    Recursive(Tree);
    out_g.close();
   if(flag<0)
       return flag;
   else
       return 1;

}

/*
int expresion(treeNode *p,int id)//p=ASSIGNMENT
{
    int tem_num=tempNum;
    int value=0;
    if(p->sonNode)
        expresion(p->sonNode,id);
    if(p->type==EXPRESSION)//根节点的表达式
    {
        QStack <treeNode*>op;
        treeNode* t=p->sonNode;
        //qDebug()<<strTokens[t->type];
        while(1)
        {
            if(t->type==RELOP||t->type==OP1||t->type==OP2)
            {
                treeNode *t1;
                treeNode *t2;
                treeNode *t3;
                t2=op[op.size()-1];
                op.pop_back();
                t1=op[op.size()-1];
                op.pop_back();
                t3=t;
                if(t3->type==OP1||t3->type==OP2)
                {
                    //int id_temp=gtable.newTemp(t);
                    out_g<<"T"<<++tempNum<<" := ";
                }
                if(t1->type==ID)
                    out_g<<"T";
                out_g<<t1->value;
                if(t3->type==RELOP)
                {
                    switch(t3->value)
                    {
                    case 0:
                        out_g<<" < ";
                        break;
                    case 1:
                        out_g<<" > ";
                        break;
                    case 2:
                        out_g<<" <= ";
                        break;
                    case 3:
                        out_g<<" >= ";
                        break;
                    case 4:
                        out_g<<" != ";
                        break;
                    case 5:
                        out_g<<" == ";
                        break;
                    default:
                        break;
                    }
                }
                if(t3->type==OP1)
                {
                     if(t3->value==0)
                         out_g<<" + ";
                     if(t3->value==1)
                         out_g<<" - ";
                }
                if(t3->type==OP2)
                {
                     if(t3->value==0)
                         out_g<<" * ";
                     if(t3->value==1)
                         out_g<<" / ";
                }
                if(t1->type==ID)
                    out_g<<"T";
                out_g<<t2->value<<endl;
                treeNode *new_node=new treeNode;
                new_node->type=ID;
                new_node->value=tem_num;
                op.push(new_node);
            }

            if(t->type==NUM||t->type==ID)
            {
                op.push(t);
            }
            if(!t->sibleNode)
                break;
            else
                t=t->sibleNode;
        }
        if(!op.empty())//说明里面还有东西
        {
            treeNode *x=op[op.size()-1];
            op.pop_back();
            out_g<<"T"<<id<<" := ";
            if(x->type==ID)
                out_g<<"T";
            out_g<<x->value<<endl;
        }
    }
    if(p->sibleNode)
        expresion(p->sibleNode,id);
    return 0;
}*/

/*
返回-1表示布尔表达式、-2表示单表达式
输入 express 以EXPRESSION为头的子树，place是如果需要跳转满足某一条件跳转到哪里。
*/
int expresion(treeNode *express, int ill) {
    char * relp[6] = {"<", "!=", "<=", ">=",">",  "=="};
    char * op1[2] = { "+", "-" };
    char * op2[2] = { "*", "/" };
    char * names[9] = { "c", "a","b","c","d", "e", "f", "g" };			//预存的符号表，名字
    stack<treeNode *, vector<treeNode*>>stk1;
    if(express->type != EXPRESSION)
        return -3;
    qDebug() << "abc" << endl;
    treeNode *temp = express->sonNode, *temp2, *temp3;
    if (temp->sibleNode == NULL)
    {
        //qDebug()<< strTokens[temp->type] << "  " << temp->value << endl;
        out_g<<names[ill]<<" := "<<temp->value<<endl;
        return -2;
    }
    int exGId;
    stk1.push(temp);

    temp = temp->sibleNode;
    stk1.push(temp);
    temp = temp->sibleNode;
    while (temp != NULL) {
        if(temp->type == RELOP){
            temp3 = new treeNode;
            temp3->type = stk1.top()->type;
            temp3->value = stk1.top()->value;
            stk1.pop();

            temp2 = new treeNode;
            temp2->type = stk1.top()->type;
            temp2->value = stk1.top()->value;
            stk1.pop();
            if(temp2->type == ID){
                if(gtable.getGId(temp2->value) == -1)
                {flag = -temp2->lineNum;
                return -4;}
            }
            if(temp3->type == ID){
                if(gtable.getGId(temp3->value) == -1)
                {flag = -temp3->lineNum;
                return -4;}
            }

            out_g<<"if ";
            if (temp2->type == TEMP) {
                out_g << " T" <<  gtable.getLId( temp2->value) << " ";
            }
            else if (temp2->type == ID)
                out_g << names[temp2->value]<<" ";
            else if (temp2->type == NUM)
                out_g << temp2->value<<" ";;
            delete temp2;
            out_g << relp[temp->value] << " ";
            if (temp3->type == TEMP) {
                out_g << "T" << gtable.getLId( temp3->value)<<" ";
            }
            else if (temp3->type == ID)
                out_g << names[temp3->value]<<" ";
            else if (temp3->type == NUM)
                out_g << temp3->value<<" ";
            delete temp3;
            out_g << "goto L" << ill <<  endl;
            return -1;
        }
        else if (temp->type == OP1 || temp->type == OP2) {
            temp3 = new treeNode;
            temp3->type = stk1.top()->type;
            temp3->value = stk1.top()->value;
            stk1.pop();

            temp2 = new treeNode;
            temp2->type = stk1.top()->type;
            temp2->value = stk1.top()->value;
            if(temp2->type == ID){
                if(gtable.getGId(temp2->value) == -1)
                {flag = -temp2->lineNum;
                return -4;}
            }
            if(temp3->type == ID){
                if(gtable.getGId(temp3->value) == -1)
                {flag = -temp3->lineNum;
                return -4;}
            }
            stk1.pop();
            temp2->sibleNode = temp3;
            temp3->sibleNode = temp;
            if (gtable.hasCur(temp2))
            {
                exGId = gtable.newTemp(temp2);
                    delete temp2;
                    delete temp3;
                temp3 = new treeNode;
                temp3->type = TEMP;
                temp3->value = exGId;
                stk1.push(temp3);
                temp = temp->sibleNode;
                continue;
            }

            exGId = gtable.newTemp(temp2);
            out_g << "T" << gtable.getLId(exGId) << " := ";
            if (temp2->type == TEMP) {
                out_g << "T" <<  gtable.getLId( temp2->value);
            }
            else if (temp2->type == ID)
                out_g << names[temp2->value]<<" ";
            else if (temp2->type == NUM)
                out_g << temp2->value<<" ";;
            delete temp2;
            switch (temp->type)
            {
            case RELOP:
                break;
            case OP1:
                out_g<<op1[temp->value] << " ";
                break;
            case OP2:
                out_g<<op2[temp->value] << " ";
                break;
            default:
                break;
            }
            if (temp3->type == TEMP) {
                out_g << "T" << gtable.getLId( temp3->value)<<" ";
            }
            else if (temp3->type == ID)
                out_g << names[temp3->value]<<" ";
            else if (temp3->type == NUM)
                out_g << temp3->value<<" ";
            delete temp3;

            {
                out_g << endl;
                temp2 = new treeNode;
                temp2->type = TEMP;
                temp2->value = exGId;
                stk1.push(temp2);
            }
        }
        else stk1.push(temp);
        temp = temp->sibleNode;
    }
    temp = stk1.top();
    int val = temp->value;
    if(temp->type == TEMP)
        delete temp;
    return gtable.getLId(val);
    return 0;
}


int _indef(treeNode *p)
{
    //qDebug()<<strTokens[p->type];
    if(p->sonNode->type==INT)
    {
        treeNode *t=p->sonNode->sibleNode;//ID
        /*赋值语句的检测*/
        if(!gtable.newTemp(t->value))//添加到符号表
            flag=1;
    }
    return 0;
}

int _assign(treeNode *p)//p=ASSIGNMENT
{
    p=p->sonNode;//p=ID
    int id=p->value;//ID
    p=p->sibleNode;//ASSIGN :=
    p = p->sibleNode;
    expresion(p,id);
    gtable.dirty(id);
    return 0;
}

int _else(treeNode *p)//p=ELSEBOCLK
{
    //qDebug()<<strTokens[p->type];
    p=p->sonNode;//p=ELSE
    p=p->sibleNode;//p=SENBLOCK
    if(p->type==SENBLOCK)
         Recursive(p);
    return 0;
}

int _if(treeNode *p)//p=IFSEN
{
    //qDebug()<<strTokens[p->type];
    p=p->sonNode;//p=EXPRESSION
    int x=0;
    int L1=gtable.newLabel();
    expresion(p,L1);
    int L2=gtable.newLabel();
    out_g<<"goto L"<<L2<<endl;
    out_g<<"L"<<L1<<": ";
    while(p->sibleNode)
    {
        p=p->sibleNode;//p=SENBLOCK
        if(p->type==SENBLOCK)
            Recursive(p);
    }
    out_g<<"L"<<L2<<": ";
    return 0;
}

int _while(treeNode *p)
{
    //qDebug()<<strTokens[p->type];
    out_g<<"if ";
    p=p->sonNode;//p=EXPRESSION
    int x=0;
    expresion(p,x);
    int L1=gtable.newLabel();
    out_g<<" goto L"<<L1<<endl;
    int L2=gtable.newLabel();
    out_g<<"goto L"<<L2<<endl;
    out_g<<"L"<<L1<<": ";
    while(p->sibleNode)
    {
        p=p->sibleNode;//p=SENBLOCK
        if(p->type==SENBLOCK)
            Recursive(p);
    }
    out_g<<"goto L"<<L1-1<<endl;
    out_g<<"L"<<L2<<": ";
    return 0;
}

/*
1.  声明语句。 newTemp， 如果返回错误，输出，x行重复定义。
2.  assign 语句， 左边的id必须在表达式计算完以后（也就是整个assign分析完以后），调用dirty函数。
3.  sentence block 进入son节点时候，调用newLevel， 离开调用backLevel。
4.  所有使用id前，如果感觉必要，都调用一下getGId（id），如果返回-1说明没有这个变量声明。
*/
int Recursive(treeNode *p)
{
    int flag=1;
    //进入语句块
   // gtable.newLevel();
    switch(p->type)
    {
    case ASSIGNMENT://赋值语句
        _assign(p);
        flag=0;
        break;
    case WHILESEN:
        flag=0;
        _while(p);
        break;
    case IFSEN:
        _if(p);
        flag=0;
        break;
    case INNERVARIDEF:
        _indef(p);
        flag=0;
        break;
    case SENBLOCK:
        //gtable.newLevel();
        Recursive(p->sonNode);
        //gtable.backLevel();
        flag=0;
        break;
    default:
        break;
    }
    //qDebug()<<strTokens[p->type]<<"   "<<p->value<<"   "<<p->lineNum;
    if(p->sonNode&&flag)
        Recursive(p->sonNode);
    if(p->sibleNode)
        Recursive(p->sibleNode);
    //退出该层前back
    return 0;
}
