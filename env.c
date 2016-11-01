#include "env.h"
#include "scheme.h"

static env_t* genv = NULL;

static env_t* get_a_env()
{
    env_t* p = (env_t*)malloc(sizeof(*p));
    if (!p)
    {
        printf("get_a_env failed \n");
        exit(1);
    }

    memset(p,0,sizeof(*p));

    return p;
}

ast*  lookup_var(char* var, env_t* env)
{
    while (env)
    {
        if (strcmp(env->name,var) == 0)
            return env->value;

       env = env->next;
    }

    return NULL;
}

env_t*    extend_env(char* var, ast* val, env_t* env)
{
     env_t* p = get_a_env();
     strcpy(p->name,var);
     p->value = val;
     p->next = env;

     if (env == genv)
     {
     		// we define something in the global env
     		// so we need keep it
     		genv = p;
     }
     return p;
}

env_t*    extend_envs(ast* vars, ast* vals, env_t* env)
{
    vars_t* pvars = (vars_t*)vars;
    if (!vars)
        return env;

    if (vals->node_t != CONS)
    {
    	return extend_env(pvars->name,vals,env);
    }

    return extend_envs((ast*)(pvars->next),vals->r,extend_env(pvars->name,vals->l,env));
}

struct env_table {
    char* func_name;
    ast*  (*func)(ast* p);
};

static ast* add(ast* p)
{
    int isFloat = 0;
    int i = 0;
    float f = 0.0;
    cons_t* q = (cons_t*)p;
    while (q && q->node_t == CONS)
    {
        if (q->carp && q->carp->node_t == FLOAT)
        {
            f += ((float_t*)q->carp)->number;
            isFloat = 1;
        }

        if (q->carp && q->carp->node_t == INT)
            i += ((num_t*)q->carp)->number;

        q = q->cdrp;
    }

    if (q && q->node_t == FLOAT)
    {
        f += ((float_t*)q)->number;
            isFloat = 1;
    }
    else if (q && q->node_t == INT)
    {
        i += ((num_t*)q)->number;
    }

    if (isFloat)
    {
        float_t* p = NULL;
        GET_A_AST(p,float_t,"add float");
        p->node_t = FLOAT;
        p->number = f + i;
        return (ast*)p;
    }
    else
    {
        num_t* p = NULL;
        GET_A_AST(p,num_t,"add num");
        p->node_t = INT;
        p->number = i;
        return (ast*)p;
    }

    return NULL;
}

static struct env_table init_env_table[] = {
    {"+",add},
    //{"-",minus},
    //{"*",mul},
    //{"/",div},
};

env_t* GET_GLOBAL_ENV()
{
    int i = 0;
	
    if (genv)
        return genv;

    for (i = 0; i < sizeof(init_env_table)/sizeof(init_env_table[0]); i++)
    {
        primitive_proc_t* p = NULL;
        GET_A_AST(p,primitive_proc_t,"GET_GLOBAL_ENV");
        p->node_t = PRIMITIVE_PROC;
        p->func = init_env_table[i].func;
        strcpy(p->name,init_env_table[i].func_name);
        p->param_len = 0;
        genv = extend_env(init_env_table[i].func_name, (ast*)p,genv);
    }

    return genv;
}
