/* Shunji Li, Laurel Orr, Sade Smith
Header file for the linked list and tokenizer classes.

NOTES TO GRADER: our error messages are more descriptive and helpful than the generic "synax error", so when comparing our output files, our errors will be different. Also, when you type q while running our tokenizer, that quits the program (we know this isn't in Dr.Racket, but we really wanted a way to quit) */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct __ConsCell {
    struct __Value *car;
    struct __Value *cdr;
} ConsCell;

typedef enum TOKEN_TYPE {
    boolType, intType, floatType, stringType, symbolType, openType, closeType, consType, primitiveType, closureType, voidType
} TOKEN_TYPE;

typedef struct __Value {
TOKEN_TYPE type; /* Make sure type includes a consType or some such */
    union data {
        int boolValue;
        int intValue;
        float floatValue;
        char *stringValue;
        char *symbolValue;
        struct __ConsCell *cons;
        struct __Value * (*primitiveValue ) (struct __Value *actuals);
        struct __Closure *closureValue;
    } data;
} Value;

typedef struct __Binding {
    char *variable;
    Value *value;
    struct __Binding *next;
} Binding;

typedef struct __Environment
{
    Binding *bindingHead;
    struct __Environment *nextEnv;

} Environment;

typedef struct __Closure
{
    Value *formals;
    Environment *env;
    Value *expr;
} Closure;

/*LINKED LIST FUNCTIONS */
/* Makes a voidType */
Value *makeVoidType();
/* Returns the length of the list */
int getCount(ConsCell *cell, int num);
/* Add item to front of list */
ConsCell *addFirst(Value *car, ConsCell *old);
/* Remove the front item from the list */
ConsCell *pop(ConsCell *old, Value **value);
/* Get last value in a list*/
Value *getLast(ConsCell *cell);
/* Deletes a value form the heap */
void deleteValue(Value *value);
/* Delete the list from the heap */
void deleteList(ConsCell *cell);
/* Reverse the list */
ConsCell *reverse(ConsCell *cell);
ConsCell *reverseHelper(ConsCell *cell, ConsCell *list);
/* Prints a value with the correct type */
void printValue(Value *value);
/* Print the cons cell */
void printConsCell(ConsCell *cell);


/* ENVIRONMENT LIST FUNCTIONS */
Environment *addFirstEnv(Binding *newEnv, Environment *old);
Environment *popEnv(Environment *old, Binding **binding);
void bind(char *var, Value *value, Environment *env);
void deleteEnvironmentList(Environment *env);
void printEnvironmentList(Environment *env);


/* BINDING LIST FUNCTIONS */
Binding *addFirstB(char *newVar, Value *newValue, Binding *old);
Binding *popB(Binding *old, char **variable, Value **value);
void deleteBindingList(Binding *binding);
void printBinding(Binding *binding);

/* TOKENIZER FUNCTIONS */
ConsCell *tokenize(char *expression);
/* Adds a token to our list of tokens */
ConsCell *addToken(ConsCell *cell, int type, char *expression, int start, int end);
/* Returns 1 if the character is an initial as defined in our tokenizer assignment */
int isInitial(char c);
/* Returns 1 if the character is a subsequent as defined in our tokenizer assignment */
int isSubsequent(char c);
/* Returns 1 if the characters is an ending character */
int isTerminator(char c);
/* Returns 1 if the quote is escaped and 0 if not */
int escapedQuote(char *expr, int c);

/* PARSER FUNCTIONS */
/* Parse the tokens */
ConsCell *parse(ConsCell *tokens, int *depth, ConsCell *tree);


/* INTERPRETER FUNCTIONS */
Value *eval(Value *expr, Environment *env);
Value *apply(Value *f, Value *args);
Environment *createTopFrame();
Value *makePrimitiveValue(Value *(*f)(Value *actuals));
Value *resolveVariable(char *symbol, Environment *env);
Value *evalEach(Value *list, Environment *env);
Value *evalCar(Value *list, Environment *env);
Value *evalCdr(Value *list, Environment *env);
Value *evalBegin(Value *list, Environment *env);
Value *evalDefine(Value *list, Environment *env);
Value *evalAnd(Value *list, Environment *env);
Value *evalOr(Value *list, Environment *env);
Value *evalIf(Value *list, Environment *env);
Value *evalCond(Value *list, Environment *env);
Value *evalCons(Value *list, Environment *env);
Value *evalAppend(Value *list, Environment *env);
Value *appendHelper(Value *list, Environment *env);
Value *evalLoad(Value *list, Environment *env);
Value *evalDisplay(Value *list, Environment *env);
void printStringDisplay(char *stringVal);
Value *evalLambda(Value *list, Environment *env);
Value *evalLet(Value *list, Environment *env);
int evalLetHelper(ConsCell *pairList, Environment *oldEnv, Environment *newEnv);
Value *evalLetStar(Value *list, Environment *env);
Environment *evalLetStarHelper(ConsCell *pairList, Environment *newEnv);
Value *evalLetRec(Value *list, Environment *env);
int letRecHelper(ConsCell *pairList, Environment *newEnv);
Value *evalSetBang(Value *list, Environment *env);
void printResult(Value *value);



/*PRIMITIVE FUNCTIONS FUNCTIONS */
Value* makeIntValue(int integer);
Value *makeFloatValue(float flt);
Value* makeBoolValue(int TrueOrFalse);
Value *add(Value *args);
Value *addHelper(Value *args);
Value *multiply(Value *args);
Value *multiplyHelper(Value *args);
Value *subtract(Value *args);
Value *subtractHelper(Value *args);
Value *division(Value *args);
Value *divisionHelper(Value *args);
Value *isNull(Value *actuals);
Value *isOdd(Value *cell);
Value *isEven(Value *cell);
Value *isZero(Value *cell);
Value *isGreaterOrEqual(Value *args);
Value *isGreater(Value *args);
Value *isLessOrEqual(Value *args);
Value *isLess(Value *args);
Value *numEqual(Value *args);
Value* isInt(Value *arg);
Value *isPair(Value *arg);
Value *not(Value *arg);
Value *modulo(Value *args);






