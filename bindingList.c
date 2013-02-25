#include "header.h"


Binding *addFirstB(char *newVar, Value *newValue, Binding *old) {
    Binding *new = (Binding *) malloc(sizeof(Binding));
    new->variable = newVar;
    new->value = newValue;
    new->next = old;
    return new;
}

Binding *popB(Binding *old, char **variable, Value **value){
    Binding *next = old->next;
    *variable = old->variable;
    *value = old->value;
    free(old);
    return next;
}

void deleteBindingList(Binding *binding){
    if (binding != NULL)
    {
        deleteBindingList(binding->next);
        free(binding->value);
        free(binding->variable);
        free(binding);
    }
    
}

void printBindingList(Binding *binding) {
    if (binding != NULL)
    {
       printBinding(binding);
       printBindingList(binding->next);
    }
}

void printBinding(Binding *binding){
    if (binding != NULL)
    {
       printf("Variable: %s\t", binding->variable);
       printValue(binding->value);
    }
}
