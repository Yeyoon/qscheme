#include "scheme.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ast* newNum(int num)
{
	struct numval* pn = NULL;
	GET_A_AST(pn,struct numval,"newNum");
	
	pn->number = num;
	pn->node_t = INT;
	
	return (ast*)pn;
}

ast* newFloat(float f)
{
	struct floatval* pf = NULL;
	GET_A_AST(pf,struct floatval, "newFloat");

	pf->number = f;
	pf->node_t = FLOAT;
	
	return (ast*)pf;
}

ast* newIfExp(ast* condition, ast* firstClause, ast* secondClause)
{
	struct ifexpval *p = NULL;
	GET_A_AST(p,struct ifexpval,"newIfExp");

	p->node_t = IF;
	p->condition = condition;
	p->first_clause = firstClause;
	p->second_clause = secondClause;
	
	return (ast*)p;	
}

ast* newDefExp(char* name,ast *defbody)
{
	struct defval *p = NULL;
	GET_A_AST(p,struct defval,"newDefExp");

	p->node_t = DEFINE;
	strcpy(p->name,name);
	p->body = defbody;

	return (ast*)p;
}

ast* newLambdaExp(ast* plist, ast* lambdaBody)
{
	struct lambdaval *p = NULL;
	GET_A_AST(p,struct lambdaval,"newLambdaExp");

	p->node_t = LAMBDA;
	p->lambda_parameters = plist;
	p->body = lambdaBody;

	return (ast*)p;
}

ast* newCons(ast *carpart,ast* cdrpart)
{
	struct consval *p = NULL;
	GET_A_AST(p,struct consval,"newCons");

	p->node_t = CONS;
	p->carp = carpart;
	p->cdrp = cdrpart;

	return (ast*)p;
}

ast* newCar(ast *carBody)
{
	struct carval *p = NULL;
	GET_A_AST(p,struct carval,"newCar");

	p->node_t = CAR;
	p->body = carBody;

	return (ast*)p;
}

ast* newCdr(ast *cdrBody)
{
	struct cdrval *p = NULL;
	GET_A_AST(p,struct cdrval,"newCdr");

	p->node_t = CDR;
	p->body = cdrBody;

	return (ast*)p;
}

ast* newList(ast *listBody)
{
	struct listval *p = NULL;
	GET_A_AST(p,struct listval,"newList");

	p->node_t = LIST;
	p->body = listBody;

	return (ast*)p;
}

ast* newQuote_l(ast* qbody)
{
	struct quoteval_l *p = NULL;
	GET_A_AST(p,struct quoteval_l,"newQuote_l");

	p->node_t = QUOTE_L;
	p->body = qbody;

	return (ast*)p;
}

ast* newQuote_n(char* name)
{
	struct quoteval_n *p = NULL;
	GET_A_AST(p,struct quoteval_n,"newQuote_n");

	p->node_t = QUOTE_N;
	strcpy(p->name,name);

	return (ast*)p;	
}

ast* newVariable(char* name)
{
	struct var *p = NULL;
	GET_A_AST(p,struct var,"newVariable");

	p->node_t = VAR;
	strcpy(p->name,name);

	return (ast*)p;	
}

ast* newVariables(char* name, ast* left)
{
	struct vars *p = NULL;
	GET_A_AST(p,struct vars,"newVariables");

	p->node_t = VARS;
	strcpy(p->name,name);
	p->next = (vars_t*)left;

	return (ast*)p;	
}

ast* newProcCall(ast* proc, ast* params)
{
	struct proccall *p = NULL;
	GET_A_AST(p,struct proccall,"newProcCall");

	// for debug
	printf("proc node type = %d\n",proc->node_t);
	p->node_t = PROC_CALL;
	p->proc = proc;
	p->proc_params = params;

	return (ast*)p;	
}

ast* car(ast* stmt)
{
	if (NULL == stmt)
	{
		printf("car stmt is NULL \n");
		return NULL;
	}

	if (stmt->node_t != CONS)
	{
		printf("can not car operater on none CONS type! \n");
		return NULL;
	}

	return (ast*)(((cons_t*)stmt)->carp);
}

ast* cdr(ast* stmt)
{
	if (NULL == stmt)
	{
		printf("cdr stmt is NULL \n");
		return NULL;
	}

	if (stmt->node_t != CONS)
	{
		printf("can not cdr operater on none CONS type! \n");
		return NULL;
	}

	return (ast*)(((cons_t*)stmt)->cdrp);
}

ast* eval(ast* stmt, env_t* env)
{
	if (NULL == stmt)
		return NULL;

	printf("H node = %d\n",stmt->node_t);
	switch (stmt->node_t)
	{
	case    INT:
	{
	        return stmt;
	 }
	       
	case      FLOAT:
	 {
	        return stmt;
	 }

	 case LAMBDA:
	 {

	       return stmt;
	 }

	 case DEFINE:
	 {
	       def_t* tmp = (def_t*)stmt;
	       env = extend_env(tmp->name,tmp->body,env);

	       /**
	         * actually we don't need the result, just extend the env
	         */
	      return stmt;
	 }

	 case VAR:
	 {
	    var_t* tmp = (var_t*)stmt;
	    return lookup_var(tmp->name,env);
	 }

	 case VARS:
	 {
	    // need not this CASE
	    return stmt;
	 }

     case CONS:
     {
	cons_t* tmp = (cons_t*)stmt;
	tmp->carp = eval(tmp->carp,env);
	tmp->cdrp = eval(tmp->cdrp,env);
        return (ast*)tmp;
     }
        
	case CAR:
	{
	    return eval(car(stmt),env);
	}

	case CDR:
       {
	    return eval(cdr(stmt),env);
	}

       case LIST:
       {
            // NOT IMPLEMENT YET
            return stmt;
       }

	case IF:
	{
	    ast* condition = NULL;
	    if_t* tmp = (if_t*)stmt;

	    condition = eval(tmp->condition,env);
	    if (!condition || ((condition->node_t == INT) && ((num_t*)condition)->number == 0))
	    {
	        return eval(tmp->second_clause,env);
	    }

	    return eval(tmp->first_clause,env);
	}

	case QUOTE_L:
	{
	    // NOT IMPLEMENT YET
	    return stmt;
	}
	
	case PROC_CALL:
	{
	    lambda_t* proc = NULL;
	    ast* parms = NULL;

	    call_t* tmp = (call_t*)stmt;

	    proc = (lambda_t*)eval(tmp->proc,env);
	    parms = eval(tmp->proc_params,env);

           if (proc->node_t == PRIMITIVE_PROC)
           {
                return ((primitive_proc_t*)proc)->func(parms);
           }

	    return eval(proc->body,extend_envs(proc->lambda_parameters,parms,env));
	}

	default:
	    printf("Not Support TYPE : %d currently\n",stmt->node_t);
	    return NULL;
	}

	return NULL;
}

static void printResult(ast* stmt)
{
	if (NULL == stmt)
	{
		printf("RESULT: STMT is NULL\n");
		return;
	}

	switch (stmt->node_t)
	{
	case    INT:
	{
	        printf("INT: %d\n",((num_t*)stmt)->number);
	 }
	       
	case      FLOAT:
	 {
	        printf("FLOAT: %f\n",((float_t*)stmt)->number);
	 }

	 case LAMBDA:
	 {

	 }

	 case DEFINE:
	 {

	 }

	 case VAR:
	 {

	 }

	 case VARS:
	 {

	 }

     case CONS:
     {

     }
        
	case CAR:
	{
	}

	case CDR:
    {
	}

       case LIST:
       {
       }

	case IF:
	{

	}

	case QUOTE_L:
	{

	}
	
	case PROC_CALL:
	{
	}

	default:
	    printf("Not Support TYPE : %d currently\n",stmt->node_t);
	    return;
	}
}
void do_run(ast* stmt)
{
    ast* res = eval(stmt,GET_GLOBAL_ENV());

    printf("Hello result \n");
    if (res)
    {
        printf("RES TYPE : %d \n", res->node_t);
    }
    else
    {
        printf("NULL RES\n");
    }
    printResult(res);
}


