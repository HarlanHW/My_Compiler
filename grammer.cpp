#include"grammer.h"
#include<stack>
#include <string.h>
string strTokens[tokenNum + LeftS + 3] = {
    "INT","VOID","ID","LP","RP", "LB", "RB", "WHILE",
    "IF", "ELSE", "RETURN", "ASSIGN", "OP1", "OP2",
    "RELOP", "DEL", "SEP", "NUM", "NL", "PROGRAM",
    "TYPE", "SENBLOCK", "INNERDEF", "INNERVARIDEF",
    "NEXT", "SENSEQ", "SENTENCE", "ASSIGNMENT", "RETURNSEN",
    "RETBLOCK",  "WHILESEN", "IFSEN", "ELSEBOCLK", "EXPRESSION",
    "COMP", "PLUSEX", "OPPLUSDEC", "TERM", "OPMULDIV", "FACTOR",
    "EPSILON", "END", "ERROR" };
string special[3] = { " | ", " |-", "   " };
string relp[6] = {"<", ">", "<=", ">=", "!=", "=="};
string op1[2] = { "Plus", "Sub" };
string op2[2] = { "Mult", "Div" };

grammer::grammer()
{
    initialize();
    makeMatrix();
}

grammer::~grammer()
{
    for (int i = 0; i<LeftS; i++) {
        delete matrix[i];
    }
    if (grammerTree != NULL) {
        delTree(grammerTree);
        grammerTree = NULL;
    }
    delete[]rule;
    delete[]matrix;

}

void grammer::initialize()
{
    //Program ::= <类型> < ID>’(‘  ‘)’<语句块>
    sentence *sen = new sentence;
    sen->next = NULL;
    sen->tokens[0] = TYPE;
    sen->tokens[1] = ID;
    sen->tokens[2] = LP;
    sen->tokens[3] = RP;
    sen->tokens[4] = SENBLOCK;
    sen->tokenLen = 5;
    rule[0].head = PROGRAM;
    rule[0].descri = sen;

    //<类型>::=int | void
    sen = new sentence;
    sentence *sen2 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = INT;
    sen->tokenLen = 1;
    sen2->next = NULL;
    sen2->tokens[0] = VOID;
    sen2->tokenLen = 1;
    rule[1].head = TYPE;
    rule[1].descri = sen;

    //<语句块> ::= ‘{‘<内部声明>  <语句串>’}’
    sen = new sentence;
    sen->next = NULL;
    sen->tokens[0] = LB;
    sen->tokens[1] = INNERDEF;
    sen->tokens[2] = SENSEQ;
    sen->tokens[3] = RB;
    sen->tokenLen = 4;
    rule[2].head = SENBLOCK;
    rule[2].descri = sen;

    //<内部声明> ::= 空 | <内部变量声明><内部声明>
    sen = new sentence;
    sen2 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = EPSILON;
    sen->tokenLen = 1;
    sen2->next = NULL;
    sen2->tokens[0] = INNERVARIDEF;
    sen2->tokens[1] = INNERDEF;
    sen2->tokenLen = 2;
    rule[3].head = INNERDEF;
    rule[3].descri = sen;

    //<内部变量声明>::=int <ID><next>;
    sen = new sentence;
    sen->next = NULL;
    sen->tokens[0] = INT;
    sen->tokens[1] = ID;
    sen->tokens[2] = NEXT;
    sen->tokens[3] = DEL;
    sen->tokenLen = 4;
    rule[4].head = INNERVARIDEF;
    rule[4].descri = sen;

    //<next> ::= ,<ID><next> | 空
    sen = new sentence;
    sen2 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = SEP;
    sen->tokens[1] = ID;
    sen->tokens[2] = NEXT;
    sen->tokenLen = 3;
    sen2->next = NULL;
    sen2->tokens[0] = EPSILON;
    sen2->tokenLen = 1;
    rule[5].head = NEXT;
    rule[5].descri = sen;

    //<语句串> ::= <语句> <语句串> | 空
    sen = new sentence;
    sen2 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = SENTENCE;
    sen->tokens[1] = SENSEQ;
    sen->tokenLen = 2;
    sen2->next = NULL;
    sen2->tokens[0] = EPSILON;
    sen2->tokenLen = 1;
    rule[6].head = SENSEQ;
    rule[6].descri = sen;

    //<语句> ::= <if语句> |< while语句> | <return语句>; | <赋值语句>;
    sen = new sentence;
    sen2 = new sentence;
    sentence *sen3 = new sentence;
    sentence *sen4 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = IFSEN;
    sen->tokenLen = 1;
    sen2->next = sen3;
    sen2->tokens[0] = WHILESEN;
    sen2->tokenLen = 1;
    sen3->next = sen4;
    sen3->tokens[0] = RETURNSEN;
    sen3->tokens[1] = DEL;
    sen3->tokenLen = 2;
    sen4->next = NULL;
    sen4->tokens[0] = ASSIGNMENT;
    sen4->tokens[1] = DEL;
    sen4->tokenLen = 2;
    rule[7].head = SENTENCE;
    rule[7].descri = sen;

    //<赋值语句> ::=  <ID> =<表达式>
    sen = new sentence;
    sen->next = NULL;
    sen->tokens[0] = ID;
    sen->tokens[1] = ASSIGN;
    sen->tokens[2] = EXPRESSION;
    sen->tokenLen = 3;
    rule[8].head = ASSIGNMENT;
    rule[8].descri = sen;

    //<return语句> ::= return  retBlock
    sen = new sentence;
    sen->next = NULL;
    sen->tokens[0] = RETURN;
    sen->tokens[1] = RETBLOCK;
    sen->tokenLen = 2;
    rule[9].head = RETURNSEN;
    rule[9].descri = sen;

    //retBlock ::= 表达式 | 空
    sen = new sentence;
    sen2 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = EXPRESSION;
    sen->tokenLen = 1;
    sen2->next = NULL;
    sen2->tokens[0] = EPSILON;
    sen2->tokenLen = 1;
    rule[10].head = RETBLOCK;
    rule[10].descri = sen;

    //<while语句> ::= while ‘( ‘<表达式> ‘)’  <语句块>
    sen = new sentence;
    sen->next = NULL;
    sen->tokens[0] = WHILE;
    sen->tokens[1] = LP;
    sen->tokens[2] = EXPRESSION;
    sen->tokens[3] = RP;
    sen->tokens[4] = SENBLOCK;
    sen->tokenLen = 5;
    rule[11].head = WHILESEN;
    rule[11].descri = sen;

    //<if语句> ::= if ‘(‘<表达式>’)’  <语句块> elseBlock
    sen = new sentence;
    sen->next = NULL;
    sen->tokens[0] = IF;
    sen->tokens[1] = LP;
    sen->tokens[2] = EXPRESSION;
    sen->tokens[3] = RP;
    sen->tokens[4] = SENBLOCK;
    sen->tokens[5] = ELSEBOCLK;
    sen->tokenLen = 6;
    rule[12].head = IFSEN;
    rule[12].descri = sen;

    //elseBlock ::= else  <语句块> | 空
    sen = new sentence;
    sen2 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = ELSE;
    sen->tokens[1] = SENBLOCK;
    sen->tokenLen = 2;
    sen2->next = NULL;
    sen2->tokens[0] = EPSILON;
    sen2->tokenLen = 1;
    rule[13].head = ELSEBOCLK;
    rule[13].descri = sen;

    //<表达式>::=<加法表达式> <comp>
    sen = new sentence;
    sen->next = NULL;
    sen->tokens[0] = PLUSEX;
    sen->tokens[1] = COMP;
    sen->tokenLen = 2;
    rule[14].head = EXPRESSION;
    rule[14].descri = sen;

    //<comp> ::= relop <加法表达式><comp> | 空
    sen = new sentence;
    sen2 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = RELOP;
    sen->tokens[1] = PLUSEX;
    sen->tokens[2] = COMP;
    sen->tokenLen = 3;
    sen2->next = NULL;
    sen2->tokens[0] = EPSILON;
    sen2->tokenLen = 1;
    rule[15].head = COMP;
    rule[15].descri = sen;

    //<加法表达式> ::= <项> <op1>
    sen = new sentence;
    sen->next = NULL;
    sen->tokens[0] = TERM;
    sen->tokens[1] = OPPLUSDEC;
    sen->tokenLen = 2;
    rule[16].head = PLUSEX;
    rule[16].descri = sen;

    //<op1> ::= +<项><op1> | 空
    sen = new sentence;
    sen2 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = OP1;
    sen->tokens[1] = TERM;
    sen->tokens[2] = OPPLUSDEC;
    sen->tokenLen = 3;
    sen2->next = NULL;
    sen2->tokens[0] = EPSILON;
    sen2->tokenLen = 1;
    rule[17].head = OPPLUSDEC;
    rule[17].descri = sen;

    //<项> ::= <因子> <op2>
    sen = new sentence;
    sen->next = NULL;
    sen->tokens[0] = FACTOR;
    sen->tokens[1] = OPMULDIV;
    sen->tokenLen = 2;
    rule[18].head = TERM;
    rule[18].descri = sen;

    //<op2> ::= * <因子> <op2> | 空
    sen = new sentence;
    sen2 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = OP2;
    sen->tokens[1] = FACTOR;
    sen->tokens[2] = OPMULDIV;
    sen->tokenLen = 3;
    sen2->next = NULL;
    sen2->tokens[0] = EPSILON;
    sen2->tokenLen = 1;
    rule[19].head = OPMULDIV;
    rule[19].descri = sen;

    //<因子> ::=num | ‘(‘<表达式>’)’ |<ID>
    sen = new sentence;
    sen2 = new sentence;
    sen3 = new sentence;
    sen->next = sen2;
    sen->tokens[0] = NUM;
    sen->tokenLen = 1;
    sen2->next = sen3;
    sen2->tokens[0] = LP;
    sen2->tokens[1] = EXPRESSION;
    sen2->tokens[2] = RP;
    sen2->tokenLen = 3;
    sen3->next = NULL;
    sen3->tokens[0] = ID;
    sen3->tokenLen = 1;
    rule[20].head = FACTOR;
    rule[20].descri = sen;

}

sentence* grammer::makerows(tokenTypes n, tokenTypes t, int cnt)
{
    //s->next=NULL;
    sentence *s = rule[n - tokenNum].descri;
    while (cnt>0) {
        s = s->next;
        cnt--;
    }
    return s;
}

void grammer::makeMatrix()
{
    sentence *error = new sentence;
    error->next = NULL;
    for (int i = 0; i<6; i++) {
        error->tokens[i] = ERROR;
        error->tokenLen = 0;
    }
    for (int i = 0; i<LeftS; i++) {
        matrix[i] = new sentence*[tokenNum];
    }

    for (int i = 0; i<LeftS; i++) {
        for (int j = 0; j<tokenNum; j++) {
            matrix[i][j] = error;
        }
    }

    matrix[PROGRAM - tokenNum][INT] = makerows(PROGRAM, INT, 0);
    matrix[PROGRAM - tokenNum][VOID] = makerows(PROGRAM, VOID, 0);

    matrix[TYPE - tokenNum][INT] = makerows(TYPE, INT, 0);
    matrix[TYPE - tokenNum][VOID] = makerows(TYPE, VOID, 1);

    matrix[SENBLOCK - tokenNum][LB] = makerows(SENBLOCK, LB, 0);

    matrix[INNERDEF - tokenNum][INT] = makerows(INNERDEF, INT, 1);
    matrix[INNERDEF - tokenNum][ID] = makerows(INNERDEF, ID, 0);
    matrix[INNERDEF - tokenNum][RB] = makerows(INNERDEF, RB, 0);
    matrix[INNERDEF - tokenNum][IF] = makerows(INNERDEF, IF, 0);
    matrix[INNERDEF - tokenNum][ELSE] = makerows(INNERDEF, ELSE, 0);
    matrix[INNERDEF - tokenNum][RETURN] = makerows(INNERDEF, RETURN, 0);

    matrix[INNERVARIDEF - tokenNum][INT] = makerows(INNERVARIDEF, INT, 0);

    matrix[NEXT - tokenNum][DEL] = makerows(NEXT, DEL, 1);
    matrix[NEXT - tokenNum][SEP] = makerows(NEXT, SEP, 0);

    matrix[SENSEQ - tokenNum][ID] = makerows(SENSEQ, ID, 0);
    matrix[SENSEQ - tokenNum][RB] = makerows(SENSEQ, RB, 1);
    matrix[SENSEQ - tokenNum][WHILE] = makerows(SENSEQ, WHILE, 0);
    matrix[SENSEQ - tokenNum][IF] = makerows(SENSEQ, IF, 0);
    matrix[SENSEQ - tokenNum][RETURN] = makerows(SENSEQ, RETURN, 0);

    matrix[SENTENCE - tokenNum][ID] = makerows(SENTENCE, ID, 3);
    matrix[SENTENCE - tokenNum][WHILE] = makerows(SENTENCE, WHILE, 1);
    matrix[SENTENCE - tokenNum][IF] = makerows(SENTENCE, IF, 0);
    matrix[SENTENCE - tokenNum][RETURN] = makerows(SENTENCE, RETURN, 2);

    matrix[ASSIGNMENT - tokenNum][ID] = makerows(ASSIGNMENT, ID, 0);

    matrix[RETURNSEN - tokenNum][RETURN] = makerows(RETURNSEN, RETURN, 0);

    matrix[RETBLOCK - tokenNum][ID] = makerows(RETBLOCK, ID, 0);
    matrix[RETBLOCK - tokenNum][LP] = makerows(RETBLOCK, LP, 0);
    matrix[RETBLOCK - tokenNum][DEL] = makerows(RETBLOCK, DEL, 1);
    matrix[RETBLOCK - tokenNum][NUM] = makerows(RETBLOCK, NUM, 0);

    matrix[WHILESEN - tokenNum][WHILE] = makerows(WHILESEN, WHILE, 0);

    matrix[IFSEN - tokenNum][IF] = makerows(IFSEN, IF, 0);

    matrix[ELSEBOCLK - tokenNum][ID] = makerows(ELSEBOCLK, ID, 1);
    matrix[ELSEBOCLK - tokenNum][RB] = makerows(ELSEBOCLK, RB, 1);
    matrix[ELSEBOCLK - tokenNum][WHILE] = makerows(ELSEBOCLK, WHILE, 1);
    matrix[ELSEBOCLK - tokenNum][IF] = makerows(ELSEBOCLK, IF, 1);
    matrix[ELSEBOCLK - tokenNum][ELSE] = makerows(ELSEBOCLK, ELSE, 0);
    matrix[ELSEBOCLK - tokenNum][RETURN] = makerows(ELSEBOCLK, RETURN, 1);

    matrix[EXPRESSION - tokenNum][ID] = makerows(EXPRESSION, ID, 0);
    matrix[EXPRESSION - tokenNum][LP] = makerows(EXPRESSION, LP, 0);
    matrix[EXPRESSION - tokenNum][NUM] = makerows(EXPRESSION, NUM, 0);

    matrix[COMP - tokenNum][RP] = makerows(COMP, RP, 1);
    matrix[COMP - tokenNum][RELOP] = makerows(COMP, RELOP, 0);
    matrix[COMP - tokenNum][DEL] = makerows(COMP, DEL, 1);

    matrix[PLUSEX - tokenNum][ID] = makerows(PLUSEX, ID, 0);
    matrix[PLUSEX - tokenNum][LP] = makerows(PLUSEX, LP, 0);
    matrix[PLUSEX - tokenNum][NUM] = makerows(PLUSEX, NUM, 0);

    matrix[OPPLUSDEC - tokenNum][RP] = makerows(OPPLUSDEC, RP, 1);
    matrix[OPPLUSDEC - tokenNum][RETURN] = makerows(OPPLUSDEC, RETURN, 1);
    matrix[OPPLUSDEC - tokenNum][OP1] = makerows(OPPLUSDEC, OP1, 0);
    matrix[OPPLUSDEC - tokenNum][RELOP] = makerows(OPPLUSDEC, RELOP, 1);
    matrix[OPPLUSDEC - tokenNum][DEL] = makerows(OPPLUSDEC, DEL, 1);

    matrix[TERM - tokenNum][ID] = makerows(TERM, ID, 0);
    matrix[TERM - tokenNum][LP] = makerows(TERM, LP, 0);
    matrix[TERM - tokenNum][NUM] = makerows(TERM, NUM, 0);

    matrix[OPMULDIV - tokenNum][RP] = makerows(OPMULDIV, RP, 1);
    matrix[OPMULDIV - tokenNum][RETURN] = makerows(OPMULDIV, RETURN, 1);
    matrix[OPMULDIV - tokenNum][OP1] = makerows(OPMULDIV, OP1, 1);
    matrix[OPMULDIV - tokenNum][OP2] = makerows(OPMULDIV, OP2, 0);
    matrix[OPMULDIV - tokenNum][RELOP] = makerows(OPMULDIV, RELOP, 1);
    matrix[OPMULDIV - tokenNum][DEL] = makerows(OPMULDIV, DEL, 1);

    matrix[FACTOR - tokenNum][ID] = makerows(FACTOR, ID, 2);
    matrix[FACTOR - tokenNum][LP] = makerows(FACTOR, LP, 1);
    matrix[FACTOR - tokenNum][NUM] = makerows(FACTOR, NUM, 0);

}

string grammer::getmargin(int margin)
{
    string s = "";
    for (int i = 0; i<margin; i++) {
        s.append("  ");
    }
    return s;
}

void grammer::SplitString(const string& s, string& v, int &c)
{
    char temp;
    istringstream str(s);
    str >> temp;
    str >> v;
    str >> temp;
    str >> c;
}

int localId = 0;
ofstream outfss;

/*
返回-1表示布尔表达式、-2表示单表达式
输入 express 以EXPRESSION为头的子树，place是如果需要跳转满足某一条件跳转到哪里。
*/

void grammer::delTree(treeNode *node) {
    if(node == NULL)
        return;
    if (node->sonNode != NULL)
        delTree(node->sonNode);
    if (node->sibleNode != NULL)
        delTree(node->sibleNode);
    delete node;
}

int grammer::analyze(string filename)
{
    if (grammerTree != NULL) {
        delTree(grammerTree);
        grammerTree = NULL;
    }
    vector<int> levelCount = vector<int>(128, 0);
    vector<int> level = vector<int>(1024, -1);
    int level2[1024];
    treeNode **stack2 = new treeNode*[1024];
    ifstream fin(filename, std::ios::in);
    if (!fin) {
        //cout << "OPEN SOURCE FILE ERROR" << endl;
        return -1;
    }
    ofstream fout(OFileName, std::ios::out);
    ofstream fout2("tree.txt", std::ios::out);
    if (!fout) {
        return -1;
    }
    string line;
    int lineNum = 1;

    int point = 0;							//始终指向栈顶元素
    int point2 = 0;
    localStack[point] = END;
    grammerTree = new treeNode();
    grammerTree->type = PROGRAM;

    localStack[++point] = PROGRAM;
    stack2[point] = grammerTree;
    level2[0] = 0;
    level[point] = 0;
    bool flag = true;
    treeNode *nodeTemp;
    getline(fin, line);
    string v;
    int attribute;
    SplitString(line, v, attribute);
    int count = getToken(v);
    while (flag) {
        while (count == NL) {					//忽略换行符
            lineNum++;
            getline(fin, line);
            SplitString(line, v, attribute);
            count = getToken(v);
        }
        if (localStack[point] < tokenNum) {
            if (localStack[point] == count) {		//即栈顶符号x与当前输入符a匹配，则将x从栈顶弹出，输入串指针后移，读入下一个符号存入a，继续对下一个字符进行分析。
                for (int i = 1; i < level[point]; i++)
                    if (levelCount[i] != 0) {
                        fout << special[0];
                        fout2 << " ";
                    }
                    else {
                        fout << special[2];
                        fout2 << " ";
                    }
                    fout << special[1];
                    fout2 << " ";
                    levelCount[level[point]]--;
                    fout << strTokens[localStack[point]];
                    fout2 << localStack[point];

                    switch (localStack[point])
                    {
                    case ID:
                    case NUM:
                    case OP1:
                    case OP2:
                    case RELOP:
                        fout << " - " << attribute << ' ';
                        fout2 << " - " << attribute << ' ';
                        break;
                    default:
                        break;
                    }
                    fout << endl;
                    fout2 << " " << lineNum << endl;
                    if (getline(fin, line)) {//读取下一个字符，若已经读至文件末尾，返回-1，证明语法正确，检验成功
                        SplitString(line, v, attribute);
                        //cout << v[1] << endl;
                        count = getToken(v);
                        point--;
                        continue;
                    }
                    else {
                        fin.close();
                        fout.close();
                        fout2.close();
                        generateTree();
                        for (int i = 0; i < 20; i++)
                            varTable.newTemp(i);
//                        outfss.open("txte.txt");
//                        treeTra(grammerTree, 0);
//                        outfss.close();
                        cout << localId << endl;
                        return 0; //正确判断的输出结果为0
                    }
            }
            else {
                flag = false;
            }
        }
        else if (localStack[point] == END) {
            if (localStack[point] == count) {
                levelCount[level[point]]--;
                point--;
                continue;
            }
            else {
                flag = false;
            }
        }
        else if (localStack[point] >= PROGRAM && localStack[point] <= FACTOR) {//栈顶为非终结符
            for (int i = 1; i < level[point]; i++)
                if (levelCount[i] != 0) {
                    fout << special[0];
                    fout2 << " ";
                }
                else {
                    fout << special[2];
                    fout2 << " ";
                }
                if (localStack[point] != PROGRAM) {
                    fout << special[1];
                    fout2 << " ";
                }

                fout << strTokens[localStack[point]];
                fout2 << localStack[point];
                switch (localStack[point])
                {
                case ID:
                case NUM:
                case OP1:
                case OP2:
                case RELOP:
                    fout << " - " << attribute << ' ';
                    fout2 << " - " << attribute << ' ';
                default:
                    break;
                }

                sentence *s = new sentence;
                s = matrix[localStack[point] - tokenNum][count];
                if (s->tokens[0] == ERROR) {//为空，则发现语法错误，调用出错处理程序进行处理
                    flag = false;
                }
                else if (s->tokens[0] == EPSILON) {//A→ε ，则只将A自栈顶弹出。
                                                   //cout<< getmargin(margin)<<"ε"<< endl;
                    fout << " - @";
                    fout2 << " - @";
                    levelCount[level[point]]--;
                    point--;
                }
                else {
                    int j = level[point];
                    levelCount[j]--;
                    /*			nodeTemp = stack2[point];*/
                    int ktemp = point;
                    for (int i = s->tokenLen - 1; i >= 0; i--) {
                        localStack[point] = s->tokens[i];
                        level[point] = j + 1;
                        levelCount[j + 1]++;
                        point++;
                    }
                    point--;

                    /*
                    treeNode *sonNode3, *sonNode2;
                    sonNode3 = sonNode2 = new treeNode;

                    for (int k = point; k > ktemp; --k) {
                    stack2[k] = sonNode3;
                    sonNode3->type = stack[k];
                    sonNode3->sibleNode = new treeNode;
                    sonNode3 = sonNode3->sibleNode;
                    }
                    sonNode3->type = stack[ktemp];
                    nodeTemp->sonNode = sonNode2;*/
                }
                fout << endl;
                fout2 << " " << lineNum << endl;
        }
    }
    //cout<<"出错行位置"<<line;
    return lineNum;
}

int grammer::getToken(string s)
{
    for (int i = 0; i <= tokenNum; i++) {
        if (s == strTokens[i])
            return i;
    }
    return -1;
}

void grammer::generateTree() {
    ifstream inf("tree.txt");
    cout << "abc" << endl;
    if (!inf) {
        cerr << "Can not open the file!" << endl;
        exit(0);
    }
    char c[1024];
    //int level[1024] = { 0 };
    treeNode *nodes[1024];
    treeNode *expNodes[1024];
    treeNode *treeTemp, *extTemp;
    int point = 0;
    int level = 0, value2;
    int expCount = 0;
    bool isExpress = false;
    bool isDeclare = false;
    bool isAssign = false;
    bool isRet = false;
    int curLevel = 0;
    int indd;
    int expLevel = 0;
    int expCurLev = 0;

    grammerTree = nodes[point] = new treeNode;
    nodes[point]->type = PROGRAM;
    point++;
    inf.getline(c, 1024);
    ++level;
    std::stack<int, vector<int>> st1;				//记录大括号匹配（比如elseBlock里面又有一层大括号，只有当elseBlock里面的大括号对应完才会返回）
    std::stack<int, vector<int>> st2;				//记录类型匹配
    int stackTp;
    st1.push(2);
    while (!inf.eof()) {
        if (isExpress) {
            level--;
            isExpress = false;
        }
        else
            inf.getline(c, 1024);
        int pot = -1;
        while (c[++pot] == ' ');
        if (!(c[pot] <= '9' && c[pot] >= '0' || c[pot] >= 'a' && c[pot] <= 'z'))
            continue;

        int value = 0;
        for (; c[pot] <= '9' && c[pot] >= '0'; pot++)
            value = value * 10 + c[pot] - '0';
        pot--;
        while (c[++pot] != '\0' && c[pot] != '@');
        if (c[pot] == '@') {
            if (value == ELSEBOCLK)
                level--;
            continue;
        }
        if (value == LB) {
            stackTp = st1.top();
            st1.pop();
            st1.push(stackTp + 1);
            level++;
            continue;
        }
        else if (value == RB) {
            stackTp = st1.top();
            st1.pop();
            if (stackTp == 1) {
                level--;
            }
            else {
                st1.push(stackTp - 1);
            }
            level--;
            continue;
        }
        else if (value == DEL) {
            if (isDeclare) {
                isDeclare = false;
                level--;
            }
            if (isAssign) {
                isAssign = false;
                level--;
            }
            if (isRet) {
                isRet = false;
                level--;
            }
            if (isExpress) {
                isExpress = false;
                level--;
            }
            continue;
        }
        else if (value == ELSEBOCLK) {
            st1.push(0);
            continue;
        }
        else if (value == WHILESEN) {
            st1.push(0);
        }
        else if (value == RP) {
            if (isExpress) {
                --expCount;
                if (expCount == 0) {
                    level--;
                    isExpress = false;
                }
            }
            continue;
        }
        else if (value == LP) {
            if (isExpress)
                ++expCount;
            continue;
        }

        if ( value == TYPE || value == IF
            || value == ELSE || value == SENSEQ || value == SENTENCE ||
            value == INNERDEF || value == WHILE || value == RETBLOCK || value == RETURN)
            continue;
        treeTemp = new treeNode;
        treeTemp->type = value;
        indd = strlen(c);
        while (c[--indd] > '9' || c[indd] < '0');
        while (c[--indd] <= '9' && c[indd] >= '0');
        value2 = 0;
        ++indd;
        while (c[indd] <= '9' && c[indd] >= '0') {
            value2 = value2 * 10 + c[indd] - '0';
            ++indd;
        }
        treeTemp->lineNum = value2;
        if (value == ID || value == NUM || value == OP1 || value == OP2
            || value == RELOP) {
            pot = -1;

            while (c[++pot] == ' ' || c[pot] == '-');
            while (c[++pot] <= '9' && c[pot] >= '0');
            while (c[++pot] == ' ' || c[pot] == '-');

            value = 0;
            for (; c[pot] <= '9' && c[pot] >= '0'; pot++)
                value = value * 10 + c[pot] - '0';
            treeTemp->value = value;
        }

        if (curLevel < level) {
            nodes[curLevel]->sonNode = treeTemp;
            nodes[level] = treeTemp;
            curLevel = level;
        }
        else if (curLevel == level) {
            nodes[curLevel]->sibleNode = treeTemp;
            nodes[curLevel] = treeTemp;
        }
        else {
            nodes[level]->sibleNode = treeTemp;
            nodes[level] = treeTemp;
            curLevel = level;
        }
        if (value == INNERVARIDEF) {
            isDeclare = true;
            ++level;
        }
        else if (value == ASSIGNMENT) {
            isAssign = true;
            ++level;
        }
        else if (value == IFSEN || value == WHILESEN ) {
            ++level;
        }
        else if (value == RETURNSEN) {
            isRet = true;
            ++level;
        }
        else if (value == EXPRESSION) {
            ++level;
            expCount = 0;
            isExpress = true;
            int pointer = -1;
            while (c[++pointer] == ' ');
            expLevel = expCurLev = pointer;
            expNodes[expCurLev] = new treeNode;
            expNodes[expCurLev]->type = EXPRESSION;
            while (true) {
                inf.getline(c, 1024);
                expLevel = -1;
                while (c[++expLevel] == ' ');
                if (expLevel <= pointer)
                    break;
                if (!(c[expLevel] <= '9' && c[expLevel] >= '0' || c[expLevel] >= 'a' && c[expLevel] <= 'z'))
                    break;
                value = 0;
                pot = expLevel;
                for (; c[pot] <= '9' && c[pot] >= '0'; pot++)
                    value = value * 10 + c[pot] - '0';
                extTemp = new treeNode;
                extTemp->type = value;
                indd = strlen(c);
                while (c[--indd] > '9' || c[indd] < '0');
                while (c[--indd] <= '9' && c[indd] >= '0');
                value2 = 0;
                ++indd;
                while (c[indd] <= '9' && c[indd] >= '0') {
                    value2 = value2 * 10 + c[indd] - '0';
                    ++indd;
                }
                extTemp->lineNum = value2;
                if (value == ID || value == NUM || value == OP1 || value == OP2
                    || value == RELOP) {
                    value = 0;
                    while (c[++pot] == ' ' || c[pot] == '-');
                    for (; c[pot] <= '9' && c[pot] >= '0'; pot++)
                        value = value * 10 + c[pot] - '0';
                    extTemp->value = value;
                }
                if (expCurLev < expLevel) {
                    expNodes[expCurLev]->sonNode = extTemp;
                    expNodes[expLevel] = extTemp;
                    expCurLev = expLevel;
                }
                else if (expCurLev == expLevel) {
                    expNodes[expCurLev]->sibleNode = extTemp;
                    expNodes[expCurLev] = extTemp;
                }
                else {
                    expNodes[expLevel]->sibleNode = extTemp;
                    expNodes[expLevel] = extTemp;
                    expCurLev = expLevel;
                }
            }
            stack<treeNode*, vector<treeNode*>>nodeSt, symbolSt;					//深度搜索栈，表达式栈
            nodeSt.push(expNodes[pointer]);
            while (!nodeSt.empty()) {
                extTemp = nodeSt.top();
                nodeSt.pop();
                if (extTemp->sonNode != NULL) {
                    extTemp = extTemp->sonNode;
                    value = extTemp->type;
                    if (value == ID || value == NUM || value == OP1 || value == OP2
                        || value == RELOP) {
                        symbolSt.push(extTemp);
                        if (extTemp->sibleNode != NULL) {
                            nodeSt.push(extTemp->sibleNode);
                            extTemp = extTemp->sibleNode;
                            if (extTemp->sibleNode != NULL) {
                                nodeSt.push(extTemp->sibleNode);
                            }
                        }
                    }
                    else {
                        nodeSt.push(extTemp);
                        if (extTemp->sibleNode != NULL) {
                            nodeSt.push(extTemp->sibleNode);
                        }
                    }
                }
            }
            if (symbolSt.empty())
                continue;
            treeTemp->sonNode = new treeNode;
            treeTemp->sonNode->value = symbolSt.top()->value;
            treeTemp->sonNode->type = symbolSt.top()->type;
            treeTemp->sonNode->lineNum = symbolSt.top()->lineNum;
            symbolSt.pop();
            extTemp = treeTemp->sonNode;
            while (!symbolSt.empty()) {
                extTemp->sibleNode = new treeNode;
                extTemp->sibleNode->value = symbolSt.top()->value;
                extTemp->sibleNode->type = symbolSt.top()->type;
                extTemp->sibleNode->lineNum = symbolSt.top()->lineNum;
                extTemp = extTemp->sibleNode;
                symbolSt.pop();
            }
            //delTree(expNodes[pointer]);
        }
    }
}
