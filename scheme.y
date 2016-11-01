%{
#include <stdio.h>
#include <stdlib.h>
#include "scheme.h"		
%}

%union {
	struct ast	*a;
	char				*str;
	int	n;
	float 	f;
}

%token <n> AST_INT
%token <f> AST_FLOAT
%token <str> AST_VAR
%type <a>	exp stmt exps if_exp lambda_exp def_exp cons_exp car_exp cdr_exp quote_exp symbols proc_call
%token EOL

%token AST_IF AST_LAMBDA AST_DEFINE AST_CAR AST_CDR AST_CONS AST_FALSE AST_TRUE  AST_QUOTE EMPTY_LIST L_BRACE R_BRACE

%%

stmt:/* empty */
	| stmt exp  { do_run($2);}
;
	
exp: if_exp { $$ = $1;}
	| lambda_exp {$$ = $1;}
	| def_exp {$$ = $1;}
	| cons_exp {$$ = $1;}
	| car_exp	 {$$ = $1;}
	| cdr_exp  {$$ = $1;}
	| quote_exp {$$ = $1;}
	| AST_INT	{ $$ = newNum($1);}
	| AST_FLOAT {$$ = newFloat($1);}
	| AST_VAR		{$$ = newVariable($1);}
	| proc_call	{$$ = $1;}
; 

if_exp: L_BRACE AST_IF exp exp exp R_BRACE  {$$ = newIfExp($3,$4,$5);}
;

lambda_exp: L_BRACE AST_LAMBDA L_BRACE symbols R_BRACE exp R_BRACE	{$$ = newLambdaExp($4,$6);}
	| L_BRACE AST_LAMBDA L_BRACE R_BRACE exp R_BRACE	{$$ = newLambdaExp(NULL,$5);}
;

def_exp: L_BRACE AST_DEFINE AST_VAR	exp R_BRACE		{$$ = newDefExp($3,$4);}
	| L_BRACE AST_DEFINE L_BRACE AST_VAR symbols R_BRACE exp R_BRACE			{$$ = newDefExp($4,newLambdaExp($5,$7));} 
	| L_BRACE AST_DEFINE L_BRACE AST_VAR R_BRACE exp R_BRACE					{$$ = newDefExp($4,newLambdaExp(NULL,$6));}
;

cons_exp: L_BRACE AST_CONS exp exp R_BRACE		{$$ = newCons($3,$4);}
;

car_exp: L_BRACE AST_CAR exp R_BRACE					{$$ = newCar($3);}
;

cdr_exp: L_BRACE AST_CDR exp R_BRACE					{$$ = newCdr($3);}
;

quote_exp: AST_QUOTE AST_VAR										{$$ = newQuote_n($2);}
	| AST_QUOTE L_BRACE exp R_BRACE							{$$ = newQuote_l($3);}
;

proc_call: L_BRACE exp exps R_BRACE				{$$ = newProcCall($2,$3);}
;

symbols: AST_VAR	{$$ = newVariables($1,NULL);}
	|AST_VAR symbols {$$ = newVariables($1,$2);}
;

exps: exp
	| exp exps   {$$ = newCons($1,$2);}

%%

int main()
{
    printf("> ");
    return yyparse();
}

 void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 }


	
