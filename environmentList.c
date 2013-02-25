#include "header.h"

Environment *addFirstEnv(Binding *bindingHead, Environment *old) {
    Environment *newEnv = (Environment *) malloc(sizeof(Environment));
    newEnv->bindingHead = bindingHead;
    newEnv->nextEnv = old;
    return newEnv;
}

Environment *popEnv(Environment *old, Binding **binding) {
    Environment *next = old->nextEnv;
    *binding = old->bindingHead;
    free(old);
    return next;
}

void bind(char *var, Value *value, Environment *env) {
    env->bindingHead = addFirstB(var, value, env->bindingHead);
}

void deleteEnvironmentList(Environment *env) {
	if (env != NULL){
        deleteEnvironmentList(env->nextEnv);
        deleteBindingList(env->bindingHead);
        free(env);
    }
}

void printEnvironmentList(Environment *env) {
    if (env != NULL)
    {
       printBindingList(env->bindingHead);
       printEnvironmentList(env->nextEnv);
    }
}