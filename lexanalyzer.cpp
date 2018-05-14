#include "lexanalyzer.h"
#include <QDebug>
LexAnalyzer::LexAnalyzer(){
}

LexAnalyzer::~LexAnalyzer() {
}

void LexAnalyzer::clearState()
{
    memset(state, 0, sizeof(state));
    memset(idStr, '\0', sizeof(idStr));
    memset(beginp, 0, sizeof(beginp));
    memset(endp, 0, sizeof(endp));
}

int LexAnalyzer::strPrintf(long begin, long end, tokenType t) //解释字符串
{
    char s[200] = "";
    long len = end - begin;
    int temp = -1;
    switch (t)
    {
    case NL:
        fprintf(target, "< NL , -1 >\n");
        break;
    case ID:
        fseek(source, -len, 1);//文件指针回退
        for (int i = 0; i<len; i++)
        {
            s[i] = fgetc(source);
        }
        temp = stable.getId(s);
        fprintf(target, "< ID , %d >\n", temp);
        break;
    case NUM:
        fseek(source, -len, 1);//文件指针回退
        for (int i = 0; i<len; i++)
        {
            s[i] = fgetc(source);
        }
        fprintf(target, "< NUM , %s >\n", s);
        break;
    case RELOP:
        fseek(source, -len, 1);//文件指针回退
        for (int i = 0; i<len; i++)
        {
            s[i] = fgetc(source);
        }
        for (int i = 0; i < 4; i++) {
            if (strcmp(s, strOP[i]) == 0) {
                temp = i + 1;
                break;
            }
        }
        if(temp != -1)
            fprintf(target, "< RELOP , %d >\n", temp);
        else
        {
            return lineNum;
        }

        break;
    default:
        fprintf(target, "< %s , -1 >\n", strToken[t]);
    }
    return 0;
}

void LexAnalyzer::unaryPrintf(char s, tokenType t) //解释一元运算符
{
    int temp;
    switch (s)
    {
    case '+':
        temp = 0;
        break;
    case '-':
        temp = 1;
        break;
    case '*':
        temp = 0;
        break;
    case '/':
        temp = 1;
        break;
    case '<':
        temp = 0;
        break;
    case '>':
        temp = 4;
        break;

    default:
        temp = -1;
        break;
    }
    fprintf(target, "< %s , %d >\n", strToken[t], temp);
}

bool LexAnalyzer::isOperator(char ch) //是否为一元运算符
{
    for (int i = 0; i<16; i++)
    {
        if (ch == unaryOP[i])
            return true;
    }
    return false;
}

int LexAnalyzer::comment(char ch, int i) // i=0
{
    bool isexit = false; //当不是注释时用于跳出循环
                         //tokenType t=COM;
    while (!feof(source))
    {
        switch (state[i]) //state[0]初始为0，之后会被赋予不同的值
        {
        case 0:
            if (ch == '/')
            {
                state[i] = 1;
                beginp[i] = ftell(source) - 1; //确定注释最开始的地方
            }
            break;
        case 1:
            if (ch == '*') //以/*开头的注释
            {
                state[i] = 2;
            }
            else if (ch == '/') {  //以//开头的注释
                state[i] = 4;
            }
            else
            {
                isexit = true; //说明不是注释，请求退出
                unaryPrintf('/', OP2);
                fseek(source, -1L, 1); //回退一个字节，因为向后判断移了一位
            }
            break;
        case 2:
            if (ch == '*')
            {
                state[i] = 3;
            }
            else
            {
                state[i] = 2;
                if (ch == '\n')
                    addNewLine();
            }
            break;
        case 3:
            if (ch == '/') //以*／为结尾的注释
            {
                state[i] = 5;
                endp[i] = ftell(source);
                isexit = true;
            }
            else state[i] = 2;
            break;
        case 4:
            if (ch == '\n') {  //以//开头的注释以\n做为结尾
                state[i] = 5;
                addNewLine();
                endp[i] = ftell(source);
                isexit = true;
            }
            else {
                state[i] = 4;
            }

            break;
        }
        if (isexit) return 0;//back;
        ch = fgetc(source);

    }
    return INT_MIN;
}

int LexAnalyzer::number(char ch, int i) // i=1
{
    beginp[i] = ftell(source) - 1;  //已经读了最开头的 回退一位重新读取
    while (!feof(source))
    {
        if (ch >= '0' && ch <= '9')
        {
            state[i] = 1;   //state[1]=1
            endp[i] = ftell(source); //记录此时的结束位置
        }
        else
        {
            fseek(source, -1L, 1); //回退,读到了下一个不是数字的字符
            return strPrintf(beginp[i], endp[i], NUM);
        }
        ch = fgetc(source);
    }
    return 0;
}

int LexAnalyzer::myOperator(char ch, int i) //i=2
{
    bool isReturn = false;
    while (!feof(source))
    {
        if (ch == '+' || ch == '-' || ch == '*' || ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '{' || ch == '}')
        {
            state[i] = 1; //state[2]=1
            if(ch == '+' || ch == '-')
                unaryPrintf(ch, OP1);
            else if(ch == '*')
                unaryPrintf(ch, OP2);
            else if (ch == ';')
                unaryPrintf(ch, DEL);
            else if (ch == ',')
                unaryPrintf(ch, SEP);
            else if (ch == '(')
                unaryPrintf(ch, LP);
            else if (ch == ')')
                unaryPrintf(ch, RP);
            else if (ch == '{')
                unaryPrintf(ch, LB);
            else if (ch == '}')
                unaryPrintf(ch, RB);

            isReturn = true;
        }
        else {
            switch (state[i]) {
            case 0: //非以上符号时
                beginp[i] = ftell(source) - 1;
                switch (ch) {
                case '<':
                    state[i] = 2;
                    break;
                case '>':
                    state[i] = 4;
                    break;
                case '=':
                    state[i] = 6;
                    break;
                case '!':
                    state[i] = 8;
                    break;
                default:
                    return lineNum;
                }
                break;

            case 2:
                if (ch == '=') { // <=
                    state[i] = 3;
                    endp[i] = ftell(source);
                    return strPrintf(beginp[i], endp[i], RELOP);
                    isReturn = true;
                }
                else {  // <
                    state[i] = 2;
                    fseek(source, -1L, 1);//回退一个字符
                    unaryPrintf('<', RELOP);
                    isReturn = true;
                }
                break;

            case 4:
                if (ch == '=') {  // >=
                    state[i] = 5;
                    endp[i] = ftell(source);
                    return strPrintf(beginp[i], endp[i], RELOP);
                    isReturn = true;
                }
                else {  // >
                    state[i] = 4;
                    fseek(source, -1L, 1);//回退一个字符
                    unaryPrintf('>', RELOP);
                    isReturn = true;
                }
                break;

            case 6:
                if (ch == '=') { // ==
                    state[i] = 7;
                    endp[i] = ftell(source);
                    return strPrintf(beginp[i], endp[i], RELOP);
                    isReturn = true;
                }
                else {  // =
                    state[i] = 6;
                    fseek(source, -1L, 1);//回退一个字符
                    unaryPrintf('=', ASSIGN);
                    isReturn = true;
                }
                break;

            case 8:
                if (ch == '=') {  // !=
                    state[i] = 9;
                    endp[i] = ftell(source);
                    return strPrintf(beginp[i], endp[i], RELOP);
                    isReturn = true;
                }
                break;
            default:
                return lineNum;
            }
        }
        if (isReturn) return 0;
        ch = fgetc(source);
    }
    return 0;
}

bool LexAnalyzer::isLiter(char ch)
{
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_') //字母或下划线
    {
        return true;
    }
    else
        return false;
}

bool LexAnalyzer::isNum(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return true;
    }
    return false;
}

int LexAnalyzer::isResw(char *s)
{
    for (int i = 0; i<6; i++)
    {
        if (strcmp(s, p[i]) == 0)
            if (i < 2)
                return i;
            else
                return i + 5;
    }
    return -1;
}

int LexAnalyzer::identifier(char ch, int i) //i=3
{
    beginp[i] = ftell(source) - 1;
    long len = 0;
    bool isQuit = false;
    while (!feof(source))
    {
        if (isLiter(ch) || isNum(ch)) //判断标识符的开始是否为字母或者下划线
        {
            state[i] = 1; //state[3]=1
            endp[i] = ftell(source);
        }
        else  //读至标识符后一个非标识符的字符
        {
            int enter = 1;
            if (ch == '\n')					//这是对于windows下面的
                ++enter;
            isQuit = true;
            fseek(source, -enter, 1); //回退一或2个字符，如果fseek在移动时会考虑\r，多记一位，ftell不会
            len = endp[i] - beginp[i];
            fseek(source, -len, 1);
            for (int j = 0; j<len; j++)
            {
                idStr[j] = fgetc(source);
            }
            int tag = isResw(idStr);
            if (tag != -1) //如果是保留字，就保存退出
            {
                return strPrintf(beginp[i], endp[i], (tokenType)tag);
            }
            else {
                return strPrintf(beginp[i], endp[i], ID);
            }
        }
        if (isQuit) return 0;
        ch = fgetc(source);
    }
    return strPrintf(beginp[i], endp[i], ID);
}

void LexAnalyzer::addNewLine()
{
    strPrintf(-1, 1, NL);
    ++lineNum;
}

int LexAnalyzer::startScanner()
{
    char ch = fgetc(source);
    int states = 0;
    while (!feof(source) && states == 0)
    {
        if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\l')
        {
            ;//nothing jump it!
        }
        else if (ch == '\n') {
            addNewLine();
            clearState();
        }
        else if (ch == '/')
        {
            comment(ch, 0);
            clearState(); //清除状态信息
        }
        else if (ch >= '0'&&ch <= '9')
        {
            states = number(ch, 1); //处理数字
            clearState();
        }
        else if (isOperator(ch))
        {
            states = myOperator(ch, 2); //处理操作符
            clearState();
        }
        else if (isLiter(ch))
        {
            states = identifier(ch, 3); //处理标志符
            clearState();
        }
        else
        {
            return lineNum;
        }
        ch = fgetc(source);
    }
    return states;
}

int LexAnalyzer::analyze(string fillName)
{
    int states;
    char s[512];
    int i;
    for(i=0;i<fillName.size();i++)
    {
        s[i]=fillName[i];
    }
    s[i]='\0';
    //qDebug()<<s;
    if ((source = fopen(s, "r")) == NULL)
    {
        return -1;
    }
    if ((target = fopen(TFNAME, "w")) == NULL)
    {
        return -2;
    }
    symbol.open(SYMFNAME, ios::out);
    if (symbol.fail()) {
        return -3;
    }
    lineNum = 1;
    states = startScanner();//开始扫描文件 entrance
    symbol << stable;
    stable.clear();
    fclose(source);
    fclose(target);
    return states;
}

