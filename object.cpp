#include "object.h"
#include <QDebug>
#include <iostream>
using namespace std;
object::object()
{
    QFile symbol("obj_table.txt");
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
int object::get_format()
{
    QFile format_file("format.txt");
    if(!format_file.open(QFile::ReadOnly|QFile::Text))
    {
        return -1;
    }
    QTextStream stream(&format_file);
    QString line;
    QStringList sections;
    format temp;
    int n = 1;
    while (!stream.atEnd())
    {
        line = stream.readLine(); // 不包括“\n”的一行文本
        //qDebug()<<line;
        sections= line.split(QRegExp("( |,)"));
        sections[1].remove("(");
        sections[4].remove(")");
        //for(int i=0;i<sections.size();i++)
        //    qDebug()<<sections[i];
        temp.line=sections[0].toInt();
        temp.op=sections[1];
        temp.op1=sections[2];
        temp.op2=sections[3];
        temp.op3=sections[4];
        format_list.push_back(temp);
    }
    format_file.close();
}

int object::object_code()
{
    QFile target("target.asm");
    if(!target.open(QFile::WriteOnly|QFile::Text))
           qDebug()<<"error";
    QTextStream out_t(&target);


    format temp;
    //format_list.size();
    //qDebug()<<format_list.size();
    for(int i=0;i<format_list.size();i++)
    {

        temp=format_list[i];
        for(int i=0;i<laberlist.size();i++)
        {
            if(temp.line==laberlist[i])
                out_t<<"L"<<temp.line<<":"<<endl;
        }
        //qDebug()<<temp.op<<endl;
        if(temp.op.toStdString()==":=")
        {
            if(temp.op1[0]>='0'&&temp.op1[0]<='9')
            {
                int reg_num=is_alloc(temp.op3);
                if(reg_num<0)//该变量未在寄存器当中,那么就加进去
                {
                    reg new_reg;
                    new_reg.name_id=namelist.indexOf(temp.op3);
                    new_reg.value=temp.op1.toInt();
                    //cout<<new_reg.name_id<<" "<<new_reg.value<<endl;
                    if(next_reg>=31)
                        next_reg=1;
                    reg_list[next_reg]=new_reg;
                    out_t<<"addi $"<<next_reg++<<" $0 "<<temp.op1.toStdString().c_str()<<endl;
                }
                else//已经在寄存器当中
                {
                    out_t<<"addi $"<<reg_num<<" $0 "<<temp.op1.toStdString().c_str()<<endl;
                }

            }
            else//寄存器之间的赋值
            {
                int reg_s=is_alloc(temp.op1);
                int reg_d=is_alloc(temp.op3);
                out_t<<"add $"<<reg_d<<" $"<<reg_s<<" $0"<<endl;
            }

        }
        else if (temp.op.toStdString()=="+")
        {
            int reg_num=is_alloc(temp.op3);
            int reg_s1=is_alloc(temp.op1);
            int reg_s2=is_alloc(temp.op2);
            if(reg_num<0)//该变量未在寄存器当中,那么就加进去
            {
                reg new_reg;
                new_reg.name_id=namelist.indexOf(temp.op3);
                new_reg.value=temp.op1.toInt();
                //cout<<new_reg.name_id<<" "<<new_reg.value<<endl;
                if(next_reg>=31)
                    next_reg=1;
                reg_list[next_reg]=new_reg;
                out_t<<"add $"<<next_reg++<<" $"<<reg_s1<<" $"<<reg_s2<<endl;
            }
            else
            {
                out_t<<"add $"<<reg_num<<" $"<<reg_s1<<" $"<<reg_s2<<endl;
            }
        }
        else if (temp.op.toStdString()=="-")
        {
            int reg_num=is_alloc(temp.op3);
            int reg_s1=is_alloc(temp.op1);
            int reg_s2=is_alloc(temp.op2);
            if(reg_num<0)//该变量未在寄存器当中,那么就加进去
            {
                reg new_reg;
                new_reg.name_id=namelist.indexOf(temp.op3);
                new_reg.value=temp.op1.toInt();
                //cout<<new_reg.name_id<<" "<<new_reg.value<<endl;
                if(next_reg>=31)
                    next_reg=1;
                reg_list[next_reg]=new_reg;
                out_t<<"sub $"<<next_reg++<<" $"<<reg_s1<<" $"<<reg_s2<<endl;
            }
            else
            {
                out_t<<"sub $"<<reg_num<<" $"<<reg_s1<<" $"<<reg_s2<<endl;
            }
        }
        else if (temp.op.toStdString()=="*")
        {
            int reg_num=is_alloc(temp.op3);
            int reg_s1=is_alloc(temp.op1);
            int reg_s2=is_alloc(temp.op2);
            if(reg_num<0)//该变量未在寄存器当中,那么就加进去
            {
                reg new_reg;
                //cout<<temp.op1.toStdString().c_str()<<temp.op2.toStdString().c_str()
                //   <<temp.op3.toStdString().c_str()<<endl;
                new_reg.name_id=namelist.indexOf(temp.op3);
                new_reg.value=temp.op1.toInt();
                cout<<namelist.size()<<endl;
                cout<<new_reg.name_id<<" "<<new_reg.value<<endl;
                if(next_reg>=31)
                    next_reg=1;
                reg_list[next_reg]=new_reg;
                //out_t<<reg_list[next_reg].name_id<<endl;
                out_t<<"mul $"<<next_reg++<<" $"<<reg_s1<<" $"<<reg_s2<<endl;
            }
            else
            {
                out_t<<"mul $"<<reg_num<<" $"<<reg_s1<<" $"<<reg_s2<<endl;
            }
        }
        else if (temp.op.toStdString()=="/")
        {
            int reg_num=is_alloc(temp.op3);
            int reg_s1=is_alloc(temp.op1);
            int reg_s2=is_alloc(temp.op2);
            if(reg_num<0)//该变量未在寄存器当中,那么就加进去
            {
                reg new_reg;
                new_reg.name_id=namelist.indexOf(temp.op3);
                new_reg.value=temp.op1.toInt();
                //cout<<new_reg.name_id<<" "<<new_reg.value<<endl;
                if(next_reg>=31)
                    next_reg=1;
                reg_list[next_reg]=new_reg;
                out_t<<"div $"<<next_reg++<<" $"<<reg_s1<<" $"<<reg_s2<<endl;
            }
            else
            {
                out_t<<"div $"<<reg_num<<" $"<<reg_s1<<" $"<<reg_s2<<endl;
            }
        }
        else if (temp.op[0]=='j')//跳转类型
        {
            laberlist.push_back(temp.op3.toInt());
            if(temp.op=="j")
            {
                out_t<<"j L"<<temp.op3.toStdString().c_str()<<endl;
            }
            else if(temp.op=="j=")
            {
                int reg_s1=is_alloc(temp.op1);
                int reg_s2=is_alloc(temp.op2);
                out_t<<"beq $"<<reg_s1<<" $"<<reg_s2<<" L"<<temp.op3.toStdString().c_str()<<endl;
            }
            else if(temp.op=="j!=")
            {
                int reg_s1=is_alloc(temp.op1);
                int reg_s2=is_alloc(temp.op2);
                out_t<<"bne $"<<reg_s1<<" $"<<reg_s2<<" L"<<temp.op3.toStdString().c_str()<<endl;
            }
            else if(temp.op=="j>")
            {
                int reg_s1=is_alloc(temp.op1);
                int reg_s2=is_alloc(temp.op2);
                //reg new_reg;
                //new_reg.name_id=namelist.indexOf(temp.op3);
                //new_reg.value=temp.op1.toInt();
                //cout<<new_reg.name_id<<" "<<new_reg.value<<endl;
                //if(next_reg>=31)
                //    next_reg=1;
                //reg_list[next_reg]=new_reg;
                //out_t<<"sub $"<<next_reg++<<" $"<<reg_s1<<" $"<<reg_s2<<endl;
                out_t<<"bgt $"<<reg_s1<<" $"<<reg_s2<<" L"<<temp.op3.toStdString().c_str()<<endl;
            }
            else if(temp.op=="j>=")
            {
                int reg_s1=is_alloc(temp.op1);
                int reg_s2=is_alloc(temp.op2);
                //reg new_reg;
                //new_reg.name_id=namelist.indexOf(temp.op3);
                //new_reg.value=temp.op1.toInt();
                //cout<<new_reg.name_id<<" "<<new_reg.value<<endl;
                //if(next_reg>=31)
                //    next_reg=1;
                //reg_list[next_reg]=new_reg;
                //out_t<<"sub $"<<next_reg++<<" $"<<reg_s1<<" $"<<reg_s2<<endl;
                out_t<<"bge $"<<reg_s1<<" $"<<reg_s2<<" L"<<temp.op3.toStdString().c_str()<<endl;
            }
            else if(temp.op=="j<")
            {
                int reg_s1=is_alloc(temp.op1);
                int reg_s2=is_alloc(temp.op2);
                //reg new_reg;
                //new_reg.name_id=namelist.indexOf(temp.op3);
                //new_reg.value=temp.op1.toInt();
                //cout<<new_reg.name_id<<" "<<new_reg.value<<endl;
                //if(next_reg>=31)
                //    next_reg=1;
                //reg_list[next_reg]=new_reg;
                //out_t<<"sub $"<<next_reg++<<" $"<<reg_s2<<" $"<<reg_s1<<endl;
                out_t<<"blt $"<<reg_s1<<" $"<<reg_s2<<" L"<<temp.op3.toStdString().c_str()<<endl;
            }
            else if(temp.op=="j<=")
            {
                int reg_s1=is_alloc(temp.op1);
                int reg_s2=is_alloc(temp.op2);
                //reg new_reg;
                //new_reg.name_id=namelist.indexOf(temp.op3);
                //new_reg.value=temp.op1.toInt();
                //cout<<new_reg.name_id<<" "<<new_reg.value<<endl;
                //if(next_reg>=31)
                //    next_reg=1;
                //reg_list[next_reg]=new_reg;
                //out_t<<"sub $"<<next_reg++<<" $"<<reg_s2<<" $"<<reg_s1<<endl;
                out_t<<"ble $"<<reg_s1<<" $"<<reg_s2<<" L"<<temp.op3.toStdString().c_str()<<endl;
            }

        }
    }
    target.close();
    return 0;
}

int object::is_alloc(QString name)
{
    int id=namelist.indexOf(name);
    if(id<=0)
        return -1;
    for(int i=0;i<32;i++)
    {
        if(reg_list[i].name_id==id)
            return i;
    }
    return -1;
}
