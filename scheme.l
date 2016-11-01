/*
 * Here is the lexical parser machine
 * for scheme
 * Author : He Zhengjun
 * Date   : 2016-10-25
 *
 */
%{
#include <stdlib.h>
#include <string.h>
#include "scheme.tab.h"
%}
DIGIT		[0-9]
CHAR		[a-zA-Z]
%%

([-+]?[1-9]{DIGIT}*)|0							{yylval.n = atoi(yytext);printf("INT: %s",yytext); return AST_INT;}

[+-]?[1-9]{DIGIT}*\.{DIGIT}*				{yylval.f = atof(yytext);printf("FLOAT: %s",yytext);return AST_FLOAT;}

[+-]?0?\.{DIGIT}*								{yylval.f = atof(yytext);printf("FLOATT: %s",yytext);return AST_FLOAT;}
"lambda"														{return AST_LAMBDA;}
"define"														{return AST_DEFINE;}
"car"															{return AST_CAR;}
"cdr"															{return AST_CDR;}
"cons"															{return AST_CONS;}
"#f"															{return AST_FALSE;}
 	
"#t"															{return AST_TRUE;}

"("																{return L_BRACE;}

")"																{return R_BRACE;}
"if"															{return AST_IF;}
 	
({CHAR}+)([+-_*/]*)({DIGIT}*)						{yylval.str = strdup(yytext); printf("VAR: %s\n",yytext);return AST_VAR;}

([+-_*]*)({CHAR}+)({DIGIT}*)						{yylval.str = strdup(yytext); printf("VARR: %s\n",yytext);return AST_VAR;}
"+"|"-"|"*"|"/"										{yylval.str = strdup(yytext); return AST_VAR;}

 
\'																	{return AST_QUOTE;}
%%
   
int yywrap()
{
	return 1;
}