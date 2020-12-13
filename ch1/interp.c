
#include <stdio.h>
#include <string.h>
#include "interp.h"
#include "util.h"

// ------------------------- Functions for name table
Table_ Table(string id, int value, Table_ tail) {
    Table_ t = checked_malloc(sizeof(*t));
    t->id = id;
    t->value = value;
    t->tail = tail;
    return t;
}

IntAndTable_ IntAndTable(int value, Table_ t) {
    IntAndTable_ intAndTable = checked_malloc(sizeof(*intAndTable));
    intAndTable->value = value;
    intAndTable->t = t;
    return intAndTable;
}

Table_ update(Table_ t, string id, int value) {
    Table_ head = t, cursor = t;
    while (cursor) {
        if (!strcmp(cursor->id, id)) {
            cursor->value = value;
            break;
        }
        cursor = cursor->tail;
    }
    if (!cursor) {
        head = insert(head, id, value);
    }
    return head;
}

Table_ insert(Table_ t, string id, int value) {
    return Table(id, value, t);
}

int lookUp(Table_ t, string id) {
    while (t) {
        if (!strcmp(t->id, id)) {
            return t->value;
        }
        t = t->tail;
    }
    return -1;
}

// ------------------------- Functions for interpreter
int maxArgsInExp(A_exp);
int maxArgsInExpList(A_expList);
int maxArgs(A_stm stm) {
    if (!stm) return 0;
    int l, r;
    switch (stm->kind) {
        case A_printStm:
            return maxArgsInExpList(stm->u.print.exps);
        case A_assignStm:
            return maxArgsInExp(stm->u.assign.exp);
        case A_compoundStm:
            l = maxArgs(stm->u.compound.stm1);
            r = maxArgs(stm->u.compound.stm2);
            return max(l, r);
    }
}

int maxArgsInExp(A_exp exp) {
    if (!exp) return 0;
    int l, r;
    switch (exp->kind) {
        case A_opExp:
            l = maxArgsInExp(exp->u.op.left);
            r = maxArgsInExp(exp->u.op.right);
            return max(l, r);
        case A_eseqExp:
            l = maxArgs(exp->u.eseq.stm);
            r = maxArgsInExp(exp->u.eseq.exp);
            return max(l, r);
        default:
            return 0;
    }
}

int maxArgsInExpList(A_expList expList) {
    if (!expList) return 0;
    if (expList->kind == A_pairExpList) {
        return 1 + maxArgsInExpList(expList->u.pair.tail);
    }
    return 1;
}


void interp(A_stm stm) {
    if (!stm) return;
    interpStm(stm, Table("~@dummy@~", -1, NULL));
}

Table_ printExp(A_exp, Table_);
Table_ interpStm(A_stm stm, Table_ table) {
    Table_ t = table;
    IntAndTable_ intAndTable;
    A_expList expList;
    switch (stm->kind) {
        case A_assignStm:
            intAndTable = interpExp(stm->u.assign.exp, t);
            t = intAndTable->t;
            t = update(t, stm->u.assign.id, intAndTable->value);
            return t;
        case A_printStm:
            expList = stm->u.print.exps;
            while (TRUE) {
                if (expList->kind == A_lastExpList) {
                    t = printExp(expList->u.last, t);
                    break;
                }
                t = printExp(expList->u.pair.head, t);
                expList = expList->u.pair.tail;
            }
            printf("\n");
            return t;
        case A_compoundStm:
            t = interpStm(stm->u.compound.stm1, table);
            t = interpStm(stm->u.compound.stm2, t);
            return t;
        default:
            return t;
    }
}

Table_ printExp(A_exp exp, Table_ table) {
    IntAndTable_ intAndTable = interpExp(exp, table);
    table = intAndTable->t;
    printf("%d ", intAndTable->value);
    return table;
}

int eval(int, A_binop, int);
IntAndTable_ interpExp(A_exp e, Table_ table) {
    IntAndTable_ l, r;
    int value;
    switch (e->kind) {
        case A_idExp:
            value = lookUp(table, e->u.id);
            l = IntAndTable(value, table);
            return l;
        case A_numExp:
            value = e->u.num;
            l = IntAndTable(value, table);
            return l;
        case A_opExp:
            l = interpExp(e->u.op.left, table);
            table = l->t;
            r = interpExp(e->u.op.right, table);
            table = r->t;
            value = eval(l->value, e->u.op.oper, r->value);
            l = IntAndTable(value, table);
            return l;
        case A_eseqExp:
            table = interpStm(e->u.eseq.stm, table);
            l = interpExp(e->u.eseq.exp, table);
            return l;
    }
    return 0;
}

int eval(int l, A_binop op, int r) {
    switch (op) {
        case A_plus:
            return l + r;
        case A_minus:
            return l - r;
        case A_times:
            return l * r;
        case A_div:
            return l / r;
        default:
            return -1;
    }
}