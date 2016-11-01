#ifndef _SCHEME_H_
#define _SCHEME_H_

#define MAX_NAME_LEN	128

typedef enum exp_type {
	INT,
	FLOAT,
	LAMBDA,
	DEFINE,
	VAR,
	VARS,
	CONS,
	CAR,
	CDR,
	LIST,
	IF,
	QUOTE_L,
	QUOTE_N,
	PROC_CALL,
	PRIMITIVE_PROC,
}node_type;

#define GET_A_AST(res,type,type_desc_str) \
	do {\
		res = (type *)malloc(sizeof(type));\
		if (NULL == res)\
		{\
			printf(type_desc_str": MALLOC ERROR \n");\
			exit(1);\
		}\
		memset(res,0,sizeof(*res));\
	}while(0);


/**
 * ast
 */
typedef struct ast {
 	node_type node_t;
 	struct ast *l;
 	struct ast *r;
}ast;

typedef struct numval {
	node_type node_t;
	int number;
}num_t;

typedef struct floatval {
	node_type node_t;
	float number;
}float_t;

typedef struct ifexpval {
	node_type node_t;
	ast		*condition;
	ast		*first_clause;
	ast		*second_clause;
}if_t;

typedef struct defval {
	node_type node_t;
	char name[MAX_NAME_LEN];
	ast	 *body;
}def_t;

typedef struct lambdaval {
	node_type node_t;
	ast				*lambda_parameters;
	ast				*body;
}lambda_t;

typedef struct consval {
	node_type node_t;
	ast				*carp;
	ast             *cdrp;
}cons_t;

typedef struct carval {
	node_type node_t;
	ast				*body;
}car_t;

typedef struct cdrval {
	node_type node_t;
	ast				*body;
}cdr_t;

typedef struct listval {
	node_type node_t;
	ast				*body;
}list_t;

typedef struct quoteval_l {
	node_type node_t;
	ast				*body;
}quote_tl;

typedef struct quoteval_n {
	node_type node_t;
	char			name[MAX_NAME_LEN];
}quote_tn;

typedef struct var {
	node_type node_t;
	char      name[MAX_NAME_LEN];
}var_t;

typedef struct vars {
	node_type node_t;
	char	  name[MAX_NAME_LEN];
	struct vars *next;
}vars_t;

typedef struct proccall {
	node_type node_t;
	ast		  *proc;
	ast		  *proc_params;
}call_t;

typedef struct primitive_proc_t {
    node_type node_t;
    char          name[MAX_NAME_LEN];
    int             param_len;
    ast*          (*func)(ast* params);
}primitive_proc_t;

ast*			newNum(int num);
ast*			newFloat(float);
ast* 		newIfExp(ast* condition, ast* firstClause, ast* secondClause);
ast*			newDefExp(char* name,ast *defbody);
ast*			newLambdaExp(ast* plist, ast* lambdaBody);
ast*			newCons(ast	*carpart, ast* cdrpart);
ast*			newCar(ast*	carBody);
ast*			newCdr(ast*	cdrBody);
ast*			newList(ast* listBody);
ast*			newQuote_l(ast* qbody);
ast*			newQuote_n(char* name); 
ast*			newVariable(char* name);
ast* 			newVariables(char* name, ast* left);
ast*			newProcCall(ast* proc, ast* params);

#endif /*_SCHEME_H_*/
