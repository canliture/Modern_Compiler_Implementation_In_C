#ifndef __INCLUDE__INTERP_H
#define __INCLUDE__INTERP_H

#include "slp.h"
#include "util.h"


typedef struct table *Table_;
typedef struct IntAndTable *IntAndTable_;

struct table { string id; int value; Table_ tail; };
struct IntAndTable { int value; Table_ t; };

/**
 * Constructor for Table_
 * @return new Table_
 */
Table_ Table(string, int, Table_);

/**
 * Constructor for IntAndTable_
 * @return
 */
IntAndTable_ IntAndTable(int, Table_);

/**
 * Update value of identifier in the value
 * @return
 */
Table_ update(Table_, string, int);

/**
 * Append pair to head of the Table
 * @return
 */
Table_ insert(Table_, string, int);

/**
 * Look up the value of a specified identifier int the Table_
 */
int lookUp(Table_, string);

/**
 * @return The max number of arguments of any `print` statement within
 *         any subexpressions of a given statement.For example, `maxArgs(prog)`
 *         is 2
 */
int maxArgs(A_stm);

/**
 * Entry for Interpret statement, it will call `interpStm`
 */
void interp(A_stm);

/**
 * Interpret statement with Table_ constructed
 * @return
 */
Table_ interpStm(A_stm, Table_);

/**
 * Interpret expression with Table_ constructed
 */
IntAndTable_ interpExp(A_exp, Table_);

#endif