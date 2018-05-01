#include "symtable.h"
#include "string.h"
SymTable::SymTable()
{
    sHead = sTail = new SUnit();
    sHead->name[0] = 0;
}

int SymTable::getId(char *name)
{
    int num = 0;
    SUnit *temp = sHead->next;
    while (temp != nullptr) {
        if (strcmp(temp->name, name) == 0)
            break;
        temp = temp->next;
        ++num;
    }
    if (temp == nullptr) {
        sTail->next = new SUnit();
        sTail = sTail->next;
        strcpy(sTail->name, name);
    }
    return num;
}

void SymTable::clear()
{
    SUnit *temp = sHead->next;
    while (temp != nullptr) {
        sHead->next = temp->next;
        delete(temp);
        temp = sHead->next;
    }
    sTail = sHead;
}
