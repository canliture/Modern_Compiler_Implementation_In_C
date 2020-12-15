#include <string.h>
#include "tokens.h"
#include "util.h"

// put code here for debug convenience
// for string literal,
char *str;
int nextPos;
int len;
void initStr() {
    len = 16 + 1;
    str = (char*)checked_malloc(len);
    str[len-1] = '\0';
    nextPos = 0;
}

void append(char ch) {
    char* newArr;
    if (nextPos >= len) {
        len = (len << 1) + 1;
        newArr = (char*)checked_malloc(len);
        newArr[len-1] = '\0';
        strncpy(newArr, str, len);
        str = newArr;
    }
    str[nextPos++] = ch;
}

string clear() {
    str[nextPos] = '\0';
    string s = String(str);
    // reset
    str = NULL;
    nextPos = 0;
    len = 0;
    return s;
}