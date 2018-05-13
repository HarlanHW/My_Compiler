#ifndef OBJECT_H
#define OBJECT_H
#include <QVector>
#include <QFile>
#include <QTextStream>

#define FORMAT "format.txt"
class reg
{
public:
    int name_id;//里面是哪个变量
    int is_using;//是否还在使用
    int is_active;//是否活跃
    int value;//寄存器的值
    reg()
    {
        name_id=-1;
        is_active=0;
        is_using=0;
        value=0;
    }
};

class format
{
public:
    int line;
    QString op;
    QString op1;
    QString op2;
    QString op3;
};

class object
{
public:
    object();
    reg reg_list[32];
    QVector<format> format_list;
    QVector<QString> namelist;
    QVector<int> laberlist;
    int next_reg=1;//从第一个寄存器开始分配
    int get_format();
    int object_code();
    int is_alloc(QString name);

};

#endif // OBJECT_H
