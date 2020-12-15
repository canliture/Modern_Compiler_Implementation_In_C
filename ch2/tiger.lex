%{
#include <string.h>
#include "util.h"
#include "tokens.h"
#include "errormsg.h"

int charPos=1;

int yywrap(void)
{
 charPos=1;
 return 1;
}


void adjust(void)
{
 EM_tokPos=charPos;
 charPos+=yyleng;
}

int commentCounter = 0;
void initStr();
void append(char);
string clear();
%}

digit       [0-9]+
identifier  [a-zA-Z]([a-zA-Z_]|{digit})*
newline     \n
whitespace  [ \r\t]

%x _STRING _COMMENT _F

%%
","	            {adjust(); return COMMA;}
":"             {adjust(); return COLON;}
";"             {adjust(); return SEMICOLON;}
"("             {adjust(); return LPAREN;}
")"             {adjust(); return RPAREN;}
"["             {adjust(); return LBRACK;}
"]"             {adjust(); return RBRACK;}
"{"             {adjust(); return LBRACE;}
"}"             {adjust(); return RBRACE;}
"."             {adjust(); return DOT;}
"+"             {adjust(); return PLUS;}
"-"             {adjust(); return MINUS;}
"*"             {adjust(); return TIMES;}
"/"             {adjust(); return DIVIDE;}
"="             {adjust(); return EQ;}
"<>"            {adjust(); return NEQ;}
"<="            {adjust(); return LE;}
"<"             {adjust(); return LT;}
">="            {adjust(); return GE;}
">"             {adjust(); return GT;}
"&"             {adjust(); return AND;}
"|"             {adjust(); return OR;}
":="            {adjust(); return ASSIGN;}
"array"         {adjust(); return ARRAY;}
"if"            {adjust(); return IF;}
"then"          {adjust(); return THEN;}
"else"          {adjust(); return ELSE;}
"while"         {adjust(); return WHILE;}
"for"	        {adjust(); return FOR;}
"to"            {adjust(); return TO;}
"do"  	        {adjust(); return DO;}
"let"  	        {adjust(); return LET;}
"in"  	        {adjust(); return IN;}
"end"  	        {adjust(); return END;}
"of"  	        {adjust(); return OF;}
"break"         {adjust(); return BREAK;}
"nil"  	        {adjust(); return NIL;}
"function"      {adjust(); return FUNCTION;}
"var"  	        {adjust(); return VAR;}
"type"          {adjust(); return TYPE;}
{identifier}    {adjust(); yylval.sval = String(yytext); return ID;}
{digit}	        {adjust(); yylval.ival = atoi(yytext); return INT;}
{newline}       {adjust(); EM_newline();}
{whitespace}    {adjust(); }

"\""            {adjust(); initStr(); BEGIN _STRING;}
"/*"            {adjust(); commentCounter = 1; BEGIN _COMMENT;}

<_STRING>{
    "\\\""                  {adjust(); append('"');}
    "\""                    {
                                adjust();
                                yylval.sval = clear();
                                BEGIN INITIAL;
                                return STRING;
                            }
    [\40-\133\135-\176]     {adjust(); append(yytext[yyleng-1]);}
    "\\n"                   {adjust(); append('\n'); }
    "\\r"                   {adjust(); append('\r'); }
    "\\t"                   {adjust(); append('\r'); }
    "\\^"[a-zA-Z]           {adjust(); /* nothing to do for control characters. */}
    "\\"([0-9]{3})          {
                                adjust();
                                int c = 1   *(yytext[yyleng-1] - '0')
                                      + 8   *(yytext[yyleng-2] - '0')
                                      + 64  *(yytext[yyleng-3] - '0');
                                if (c > 127) EM_error(EM_tokPos, "\\ddd over range ascii.");
                                else append(c);
                            }
    "\\\\"                  {adjust(); append('\\');}
    "\\f"                   {adjust(); BEGIN _F;}
    .                       {adjust(); EM_error(EM_tokPos,"illegal token");}
    <<EOF>>                 {EM_error(EM_tokPos,"EOF in string"); return 0;}
}

<_F>{
    "f\\"                   {adjust(); BEGIN _STRING;}
    {newline}               {adjust(); EM_newline();}
    {whitespace}            {adjust(); }
    .                       {adjust(); EM_error(EM_tokPos,"illegal token"); }
    <<EOF>>                 {EM_error(EM_tokPos,"EOF in string"); return 0;}
}

<_COMMENT>{
    "*/"                    {adjust(); commentCounter--; if(!commentCounter) BEGIN INITIAL;}
    "/*"                    {adjust(); commentCounter++;}
    {newline}               {adjust(); EM_newline();}
    {whitespace}            {adjust(); }
    .                       {adjust(); }
    <<EOF>>                 {EM_error(EM_tokPos,"EOF in comment"); return 0;}
}

.               {adjust(); EM_error(EM_tokPos,"illegal token");}
<<EOF>>         {return 0;}
