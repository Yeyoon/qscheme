#ifndef __ENV_H__
#define __ENV_H__

#include "scheme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct env_t_{
    char name[MAX_NAME_LEN];
    ast*  value;
    struct env_t_* next;
}env_t;

ast*  lookup_var(char* var, env_t* env);
env_t*    extend_env(char* var, ast* val, env_t* env);
env_t*    extend_envs(ast* vars, ast* vals, env_t* env);
env_t*	GET_GLOBAL_ENV();

#endif
