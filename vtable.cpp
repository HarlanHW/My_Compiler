#include"vtable.h"

#define CONVERTTOHASH(l, r, op) ((((__int64) l) <<34) | (((__int64)r) << 4) | (op & 15))
#define EXTRELEFT 0xfffffffc00000000
#define EXTRERIGHT 0x00000003fffffff0

VTable::~VTable(){
    for (int i = 0; i < curLevel; ++i) {
        if(vItems[i] != NULL)
            delNodes(i);
    }
}

void VTable::delNodes(int pos) {
    VNode *temp;
    while (vItems[pos] != NULL) {
        temp = vItems[pos];
        vItems[pos] = vItems[pos]->next;
        delete temp;
    }
}

/*bool VTable::isValid(int id) {
    VNode *temp;
    for (int i = curLevel; i > -1; --i) {
        temp = vItems[i];
        while (temp != NULL) {
            if (!temp->isTemp && temp->lID == id)
                return true;
            temp = temp->next;
        }
    }
    return false;
}*/

int VTable::getGId(int id) {
    VNode *temp;
    for (int i = curLevel; i > -1; --i) {
        temp = vItems[i];
        while (temp != NULL) {
            if (!temp->isTemp && temp->lId == id)
                return temp->gId;
            temp = temp->next;
        }
    }
    return -1;
}

/*
 *item是一个后缀表达式，前两个是操作数，第三个是操作符，
 *首先操作数要区分id和value，对于NUM类型，id1，id2的值为 value<<1 | 1; ID、TEMP类型值为 value<<1
 *输入的后缀表达式为第一个 op 第二个 返回负一说明表达式不对
 */

int VTable::newTemp(treeNode *item) {
    treeNode *temp1;
    temp1 = item->sibleNode;
    int id1, id2;

    switch (item->type)
    {
    case ID:
        id1 = getGId(item->value) << 1;
        break;
    case TEMP:
        id1 = item->value << 1;
        break;
    case NUM:
        id1 = item->value << 1 | 1;
        break;
    default:
        return -1;
        break;
    }
    switch (temp1->type)
    {
    case ID:
        id2 = getGId(temp1->value) << 1;
        break;
    case TEMP:
        id2 = temp1->value << 1;
        break;
    case NUM:
        id2 = temp1->value << 1 | 1;
        break;
    default:
        return -1;
        break;
    }

    temp1 = temp1->sibleNode;
    bool flag = false;
    int opCode = 0;						//RELOP 为原始值， OP1 OP2随后
    if (temp1->type == RELOP) {
        opCode = temp1->value;
        flag = opCode > 3;
    }
    else if (temp1->type == OP1) {
        flag = (temp1->value == 0);
        opCode = temp1->value + 6;
    }
    else if (temp1->type == OP2) {
        flag = (temp1->value == 0);
        opCode = temp1->value + 8;
    }
    else
        return -1;
    __int64 value;
    int ret;
    if (flag) {										//对称的操作符
        value = CONVERTTOHASH(id2, id1, opCode);
        ret = myHash(value);
        if (ret > -1)
            return ret;
    }
    value = CONVERTTOHASH(id1, id2, opCode);
    ret = myHash(value);
    if (ret > -1)
        return ret;
    VNode *vtemp = new VNode;
    vtemp->hashValue = value;
    vtemp->gId = allGId++;
    vtemp->lId = allLId++;
    vtemp->isTemp = true;
    vtemp->next = vItems[curLevel];
    vItems[curLevel] = vtemp;
    return vtemp->gId;
}

int VTable::myHash(__int64 val) {
    VNode *temp;
    for (int i = curLevel; i > -1; --i) {
        temp = vItems[i];
        while (temp != NULL) {
            if (temp->isTemp && temp->hashValue == val)
                return temp->gId;
            temp = temp->next;
        }
    }
    return -1;
}

bool VTable::newTemp(int id) {
    if (hasCur(id))
        return false;
    VNode *temp = new VNode;
    temp->gId = allGId++;
    temp->lId = id;
    temp->next = vItems[curLevel];
    vItems[curLevel] = temp;
    return true;
}

void VTable::dirty(int id) {
    int gId = getGId(id);
    if (gId == -1)
        return;
    VNode *temp, *temp2;
    __int64 hval1, hval2;
    std::stack<int, std::vector<int>> st1;
    st1.push(gId);

    while (!st1.empty()) {
        gId = st1.top();
        st1.pop();
        hval1 = CONVERTTOHASH(gId, 0, 0);
        hval2 = CONVERTTOHASH(0, gId, 0);
        for (int i = curLevel; i > -1; --i) {
            temp2 = temp = vItems[i];
            while (temp != NULL) {
                if (temp->isTemp && ((temp->hashValue&EXTRELEFT) == hval1 || (temp->hashValue&EXTRERIGHT) == hval2)) {
                    st1.push(temp->gId);
                    if (temp == vItems[i]) {
                        temp2 = vItems[i] = temp->next;
                        delete temp;
                        temp = vItems[i];
                    }
                    else {
                        temp2->next = temp->next;
                        delete temp;
                        temp = temp2;
                    }
                }
                else {
                    temp2 = temp;
                    temp = temp->next;
                }
            }
        }
    }
}

void VTable::newLevel() {
    curLevel++;
}

void VTable::backLevel() {
    delNodes(curLevel);
    --curLevel;
}

bool VTable::hasCur(int id) {
    VNode *temp = vItems[curLevel];
    while (temp!=NULL)
    {
        if (!temp->isTemp && temp->lId == id)
            return true;
        temp = temp->next;
    }
    return false;
}

int VTable::getLId(int id) {
    VNode *temp;
    for (int i = curLevel; i > -1; --i) {
        temp = vItems[i];
        while (temp != NULL) {
            if (temp->isTemp && temp->gId == id)
                return temp->lId;
            temp = temp->next;
        }
    }
    return -1;
}

bool VTable::hasCur(treeNode *temp) {
    treeNode *temp1;
    temp1 = temp->sibleNode;
    int id1, id2;

    switch (temp->type)
    {
    case ID:
        id1 = getGId(temp->value) << 1;
        break;
    case TEMP:
        id1 = temp->value << 1;
        break;
    case NUM:
        id1 = temp->value << 1 | 1;
        break;
    default:
        return false;
        break;
    }
    switch (temp1->type)
    {
    case ID:
        id2 = getGId(temp1->value) << 1;
        break;
    case TEMP:
        id2 = temp1->value << 1;
        break;
    case NUM:
        id2 = temp1->value << 1 | 1;
        break;
    default:
        return false;
        break;
    }

    temp1 = temp1->sibleNode;
    bool flag = false;
    int opCode = 0;						//RELOP 为原始值， OP1 OP2随后
    if (temp1->type == RELOP) {
        opCode = temp1->value;
        flag = opCode > 3;
    }
    else if (temp1->type == OP1) {
        flag = (temp1->value == 0);
        opCode = temp1->value + 6;
    }
    else if (temp1->type == OP2) {
        flag = (temp1->value == 0);
        opCode = temp1->value + 8;
    }
    else
        return false;
    __int64 value;
    int ret;
    if (flag) {										//对称的操作符
        value = CONVERTTOHASH(id2, id1, opCode);
        ret = myHash(value);
        if (ret > -1)
            return true;
    }
    value = CONVERTTOHASH(id1, id2, opCode);
    ret = myHash(value);
    if (ret > -1)
        return true;
    return false;
}


