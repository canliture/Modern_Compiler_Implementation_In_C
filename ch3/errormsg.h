#ifndef  __INCLUDE_ERRORMSG_H
#define __INCLUDE_ERRORMSG_H

extern bool EM_anyErrors;

void EM_newline(void);

extern int EM_tokPos;

void EM_error(int, string,...);
void EM_impossible(string,...);
void EM_reset(string filename);

#endif