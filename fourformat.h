#ifndef FOURFORMAT_H
#define FOURFORMAT_H

#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <vector>
#include <QDebug>
#include <iostream>
#define StartAddr 0  //程序的入口地址
#define MAX 100        //标号个数
#define FILEIN "generate.txt"
#define FILEOUT "format.txt"
using namespace std;

typedef struct{  //标号代表的数据结构，由标号名称和标号行所在的地址组成
    string Lname;
    int Laddr;
} Label;



class Fourformat
{
public:
    Label labellist[MAX]={};
    int cnt = 0;
    //Fourformat();

    //遍历文件，找到标号所代表的地址行，填充labellist数组，方便后续寻找
    void detectLabel(vector<string>& v, int addr)
    {
        int i;
        if(v[0][0]=='L'){
            for(i=0;i<=cnt;i++){
                if(v[0]==labellist[i].Lname)
                    break;
            }
            if(i>cnt){
                labellist[cnt].Lname=v[0];
                labellist[cnt].Laddr=addr;
                cnt++;
            }
        }
    }



    //用特定的符号分割字符串
    void SplitString(const string& s, vector<string>& v, const string& c)
    {
        string::size_type pos1, pos2;
        pos2 = s.find(c);
        pos1 = 0;
        while(string::npos != pos2)
        {
            v.push_back(s.substr(pos1, pos2-pos1));

            pos1 = pos2 + c.size();
            pos2 = s.find(c, pos1);
        }
        if(pos1 != s.length())
            v.push_back(s.substr(pos1));
    }

    void getfourformat(string file)
    {
        ifstream infile;
        infile.open(file.data());   //将文件流对象与文件连接起来
        assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行

        string s;
        int num=StartAddr;
        while(getline(infile,s)) //第一次遍历文件，生成标号数组labellist
        {
            if(s.size()<5)
                break;
            vector<string> line;
            SplitString(s, line, " ");
            detectLabel(line,num);
            num++;

        }

        infile.close();
        infile.open(file.data());   //将文件流对象与文件连接起来
        assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行

        ofstream outfile;
        outfile.open(FILEOUT);   //作为输出文件打开
        num=StartAddr;
        string code;
        while(getline(infile,s)) //第二次遍历文件，生成四元式
        {
            if(s.size()<5)
                break;
            vector<string> line;
            SplitString(s, line, " ");
            fourformat(line, num, code);
            outfile<<code;
            num++;
        }
        outfile.close();
    }

    //生成四元式
    void fourformat(vector<string>& v, int addr, string &code)
    {
        int i, jaddr;
        if(v[0][0]=='L')  //去掉标号带来的影响
            v.erase(v.begin());
        stringstream ss;

        if(v[0]=="goto"){ //goto语句，直接jump，生成addr (j,-,-,jaddr)的格式
            qDebug()<<QString::fromStdString(v[0])<<QString::fromStdString(v[1]);
            string temp=v[1]+":";
            for(i=0;i<=cnt;i++){
                if(temp==labellist[i].Lname){
                    jaddr = labellist[i].Laddr;
                    //cout << addr << " (j,-,-," << jaddr <<")" << endl;
                    ss << addr << " (j,-,-," << jaddr <<")" << endl;
                    code=ss.str();
                }
            }
        }
        else if(v[0]=="if"){ //if语句，条件转移，生成addr (jrelop,x,y,jaddr)，relop为< > <= >= == !=
            string temp=v[v.size()-1]+":";
            for(i=0;i<=cnt;i++){
                if(temp==labellist[i].Lname){
                    jaddr = labellist[i].Laddr;
                    //cout << addr << " (j" << v[v.size()-4] << "," << v[v.size()-5] << "," << v[v.size()-3] << "," << jaddr << ")" << endl;
                    ss << addr << " (j" << v[v.size()-4] << "," << v[v.size()-5] << "," << v[v.size()-3] << "," << jaddr << ")" << endl;
                    code=ss.str();
                }
            }
        }
        else if(v[1]==":="){ //赋值语句
            if(v.size()==3){ //直接赋值，三个参数
                //cout << addr << " (" << v[1] << "," << v[2] << ",-," << v[0] << ")" << endl;
                ss << addr << " (" << v[1] << "," << v[2] << ",-," << v[0] << ")" << endl;
                code=ss.str();
            }
            else{ //计算赋值，四个参数
                //cout << addr << " (" << v[3] << "," << v[2] << "," << v[4] << "," << v[0] << ")"<< endl;
                ss << addr << " (" << v[3] << "," << v[2] << "," << v[4] << "," << v[0] << ")"<< endl;
                code=ss.str();
            }
        }
    }

};
#endif // FOURFORMAT_H
