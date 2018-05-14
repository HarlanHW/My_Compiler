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
int tempNum=0;
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
        string s=file_s.readLine().toStdString();
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

int Generate::expresion(treeNode *p,int id)//p=ASSIGNMENT
{
    int tem_num=tempNum;
    int value=0;
    if(p->sonNode)
        expresion(p->sonNode,id);
    if(p->type==EXPRESSION)//根节点的表达式，后缀表达式
    {
        QStack <treeNode*>op_code;
        QStack <treeNode*>op_num;
        treeNode* t=p->sonNode;
        while(1)
        {
            if(t->type==RELOP||t->type==OP1||t->type==OP2)
                op_code.push_back(t);
            if(t->type==NUM||t->type==ID)
            {
                op_num.push_back(t);
            }
            if(!t->sibleNode)
                break;
            else
                t=t->sibleNode;

        }
        //qDebug()<<op_code.size()<<op_num.size();
        while(op_num.size()&&op_code.size())
        {
            //t=p->sonNode;
            //if(t->type==RELOP||t->type==OP1||t->type==OP2)
            {
                treeNode *t1;
                treeNode *t2;
                treeNode *t3;
                t1=op_num[0];
                op_num.remove(0);
                t2=op_num[0];
                op_num.remove(0);
                t3=op_code[0];
                op_code.remove(0);

                if(t3->type==OP1||t3->type==OP2)
                {
                    //int id_temp=gtable.newTemp(t);
                    //生成新ID,新节点
                    char new_id[4];
                    new_id[0]='T';new_id[3]=0;
                    itoa(tempNum,(new_id+1),10);
                    namelist.push_back(new_id);
                    treeNode *new_top=new treeNode;
                    new_top->lineNum=t1->lineNum;
                    new_top->type=ID;
                    new_top->value=namelist.size()-1;
                    op_num.push_front(new_top);
                    out_g<<"T"<<tempNum++<<" := ";

                }
                if(t3->type==RELOP)
                {
                    out_g<<"L"<<laberlist[0]<<": if ";
                }
                if(t1->type==ID)
                    out_g<<namelist[t1->value];

                if(t3->type==RELOP)
                {
                    switch(t3->value)
                    {
                    case 5:
                        out_g<<" < ";
                        break;
                    case 4:
                        out_g<<" > ";
                        break;
                    case 3:
                        out_g<<" <= ";
                        break;
                    case 2:
                        out_g<<" >= ";
                        break;
                    case 1:
                        out_g<<" != ";
                        break;
                    case 0:
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
                //qDebug()<<t2->value;
                if(t2->type==ID)
                    out_g<<namelist[t2->value]<<" ";
                if(t3->type!=RELOP)
                    out_g<<endl;
                //if(t3->value==RELOP)
                  //  out_g<<"goto ";
            }

        }
        if(!op_num.empty())//说明里面还有东西
        {
            qDebug()<<op_num.size();
            treeNode *x=op_num[op_num.size()-1];
            op_num.pop_back();
            qDebug()<<"**"<<strTokens[x->type];
            qDebug()<<QString::fromStdString(namelist[x->value]);
            out_g<<namelist[id]<<" := ";
            if(x->type==ID)
            {

                 out_g<<namelist[x->value]<<endl;
            }
            else
            {
                out_g<<x->value<<endl;
            }

            //out_g<<x->value<<endl;
        }
    }
    //if(p->sibleNode)
    //    expresion(p->sibleNode,id);
    return 0;
}

/*
返回-1表示布尔表达式、-2表示单表达式
输入 express 以EXPRESSION为头的子树，place是如果需要跳转满足某一条件跳转到哪里。
*/

int Generate::_indef(treeNode *p)
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

int Generate::_assign(treeNode *p)//p=ASSIGNMENT
{
    p=p->sonNode;//p=ID
    int id=p->value;//id的值
    p=p->sibleNode;//ASSIGN :=
    p = p->sibleNode;
    expresion(p,id);
    //gtable.dirty(id);
    return 0;
}

int Generate::_if(treeNode *p)//p=IFSEN
{
    //qDebug()<<strTokens[p->type];
    p=p->sonNode;//p=EXPRESSION
    int x=0;
    int L0=gtable.newLabel();
    int L1=gtable.newLabel();
    int L2=gtable.newLabel();
    laberlist[0]=L0;
    laberlist[1]=L1;
    laberlist[2]=L2;
    expresion(p,L1);

    out_g<<"goto L"<<L1<<endl;

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

int Generate::_else(treeNode *p)//p=ELSEBOCLK
{
    //qDebug()<<strTokens[p->type];
    p=p->sonNode;//p=ELSE
    p=p->sibleNode;//p=SENBLOCK
    if(p->type==SENBLOCK)
         Recursive(p);
    return 0;
}



int Generate::_while(treeNode *p)
{
    //qDebug()<<strTokens[p->type];
    out_g<<"if ";
    p=p->sonNode;//p=EXPRESSION
    int x=0;
    expresion(p,x);
    int L1=gtable.newLabel();
    out_g<<"goto L"<<L1<<endl;
    int L2=gtable.newLabel();
    out_g<<"goto L"<<L2<<endl;
    out_g<<"L"<<L1<<": "<<endl;
    while(p->sibleNode)
    {
        p=p->sibleNode;//p=SENBLOCK
        if(p->type==SENBLOCK)
            Recursive(p);
    }
    //out_g<<"goto L"<<L1-1<<endl;
    out_g<<"L"<<L2<<": "<<endl;
    return 0;
}

/*
1.  声明语句。 newTemp， 如果返回错误，输出，x行重复定义。
2.  assign 语句， 左边的id必须在表达式计算完以后（也就是整个assign分析完以后），调用dirty函数。
3.  sentence block 进入son节点时候，调用newLevel， 离开调用backLevel。
4.  所有使用id前，如果感觉必要，都调用一下getGId（id），如果返回-1说明没有这个变量声明。
*/
int Generate::Recursive(treeNode *p)
{
    //qDebug()<<strTokens[p->type]<<"   "<<p->value<<"   "<<p->lineNum;
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

    if(p->sonNode&&flag)
        Recursive(p->sonNode);
    if(p->sibleNode)
        Recursive(p->sibleNode);
    //退出该层前back
    return 0;
}
