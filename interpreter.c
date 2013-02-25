#include "header.h"

#define BUZZ_SIZE 1024

Value *eval(Value *expr, Environment *env){
    switch (expr->type) { //will only be symbol, number, string, or symbol
        case boolType:
            return expr;

        case intType:
        	return expr;
        	
        case floatType:
        	return expr;
        	
        case stringType:
        	return expr;
        
		  case symbolType:
			return resolveVariable(expr->data.symbolValue, env);

        case consType: {
            /* empty list */
            if (expr->data.cons == NULL) {
                return expr;
            }
            /* function application or special form */
            ConsCell *cell = expr->data.cons;

            /* If it's a special form, the car will be a symbol. */
            if (cell->car->type == symbolType) {
                char *symbol = cell->car->data.symbolValue;
                if (!strcmp(symbol, "define")) {
                    return evalDefine(cell->cdr, env);
                }
                else if (!strcmp(symbol, "if")) {
                    return evalIf(cell->cdr, env);
                }
                else if (!strcmp(symbol, "quote")) {
                    if (cell->cdr->data.cons == NULL) {
                        printf("Warning: must provide arguments to quote\n");
                        return NULL;
                    }
                    if (cell->cdr->data.cons->cdr->data.cons == NULL) {
                    	return cell->cdr->data.cons->car;
                    }
                    printf("Warning: must only have one argument to quote\n");
                    return NULL;
                }
                else if (!strcmp(symbol, "let")) {
                    return evalLet(cell->cdr, env);
                }
                else if (!strcmp(symbol, "lambda")) {
                    return evalLambda(cell->cdr, env);
                }
                else if (!strcmp(symbol, "load")) {
                    return evalLoad(cell->cdr, env);
                }
                else if (!strcmp(symbol, "let*")) {
                    return evalLetStar(cell->cdr, env);
                }
                else if (!strcmp(symbol, "letrec")) {
                    return evalLetRec(cell->cdr, env);
                }
                else if (!strcmp(symbol, "set!")) {
                    return evalSetBang(cell->cdr, env);
                }
				else if (!strcmp(symbol, "display")) {
                    return evalDisplay(cell->cdr, env);
                }
				else if (!strcmp(symbol, "list")) {
		            return evalEach(cell->cdr, env);
		        }
				else if (!strcmp(symbol, "car")) {
                    return evalCar(cell->cdr, env);
		        }
				else if (!strcmp(symbol, "cdr")) {
                    return evalCdr(cell->cdr, env);
            	}
                else if(!strcmp(symbol, "append")) {
                    return evalAppend(cell->cdr, env);
                }
                else if(!strcmp(symbol, "cons")) {
                    return evalCons(cell->cdr, env);
                }
                else if(!strcmp(symbol, "and")) {
                    return evalAnd(cell->cdr, env);
                }
                else if(!strcmp(symbol, "or")) {
                    return evalOr(cell->cdr, env);
                }
                else if(!strcmp(symbol, "cond")) {
                    return evalCond(cell->cdr, env);
                }
                else if(!strcmp(symbol, "begin")) {
                    return evalBegin(cell->cdr, env);
                }
				else if (!strcmp(symbol, "length")) {
					if (cell->cdr->data.cons != NULL && cell->cdr->data.cons->cdr->data.cons == NULL) {
						Value *value = eval(cell->cdr->data.cons->car, env);
						if(!value || value->type != consType) {
							printf("Warning: the argument must be a list\n");
							return NULL;
						}
						int i = 0;
						ConsCell *cons = value->data.cons;
						while(cons){
							i++;
							if(cons->cdr->type != consType) {
								printf("Warning: the argument must be a list\n");
								return NULL;
							}
							cons = cons->cdr->data.cons;
						}
						Value *newValue = (Value *) malloc(sizeof(Value));
                    	newValue->type = intType;
						newValue->data.intValue = i;
                    	return newValue;
					}
                    printf("Warning: must provide one only one argument to length\n");
					return NULL;	
				}
            }
            /* Otherwise it's an application. */
            Value *f = eval(cell->car, env);
            Value *args = evalEach(cell->cdr, env);
            return apply(f, args);
        }
    }

}

Value *apply(Value *f, Value *args){
    //no arguments will be represented is args->type = consType and args->data.cons = NULL
    if (f == NULL || args == NULL) {
        printf("Warning: function or arguments is bad\n");
        return NULL;
    }
    if (args->type != consType) {
        return NULL;
    }
    if (f->type == primitiveType)
    {
        return f->data.primitiveValue(args);
    }
    else if (f->type == closureType)
    {
        Environment *env = addFirstEnv(NULL, f->data.closureValue->env);
        Value *formals = f->data.closureValue->formals;
        if (formals == NULL || formals->type != consType) {
            return NULL;
        }
        while (formals->data.cons && args->data.cons){
            if (formals->data.cons->car->type != symbolType) {
                printf("Warning: the formal needs to be a symbol but is a %d\n", formals->data.cons->car->type);
                return NULL;
            }
            env->bindingHead = addFirstB(formals->data.cons->car->data.symbolValue, args->data.cons->car, env->bindingHead);
            formals = formals->data.cons->cdr;
            args = args->data.cons->cdr;
        }
        if(formals->data.cons || args->data.cons) {
            printf("Warning: formals and arguments lengths don't match\n");
            return NULL;
        }
        Value *temp = eval(f->data.closureValue->expr, env);
        return temp;
    }
    printf("Warning: function called on non function names! \n");
    return NULL;
}

Environment *createTopFrame(){
    Environment *frame = addFirstEnv(NULL, NULL); //create a new frame is pointing to NULL and as no bindings
    bind("+", makePrimitiveValue(add), frame);
	bind("null?", makePrimitiveValue(isNull), frame);
    bind("-", makePrimitiveValue(subtract), frame);
    bind("*", makePrimitiveValue(multiply), frame);
    bind("even?", makePrimitiveValue(isEven), frame);
    bind("odd?", makePrimitiveValue(isOdd), frame);
    bind("zero?", makePrimitiveValue(isZero), frame);
    bind("/", makePrimitiveValue(division), frame);
    bind(">=", makePrimitiveValue(isGreaterOrEqual), frame);
    bind("<=", makePrimitiveValue(isLessOrEqual), frame);
    bind("=", makePrimitiveValue(numEqual), frame);
    bind("integer?", makePrimitiveValue(isInt), frame);
    bind("pair?", makePrimitiveValue(isPair), frame);
    bind("not", makePrimitiveValue(not), frame);
    bind("modulo", makePrimitiveValue(modulo), frame);
    bind(">", makePrimitiveValue(isGreater), frame);
    bind("<", makePrimitiveValue(isLess), frame);
	return frame;
}

Value *makePrimitiveValue(Value *(*f)(Value *actuals)){
    Value *ret = (Value *) malloc(sizeof(Value));
    ret->type = primitiveType;
    ret->data.primitiveValue = f;
    return ret;
}

Value *resolveVariable(char *symbol, Environment *env){
        if (env != NULL) {    
            Binding *dummyBinding = env->bindingHead;
            while (dummyBinding){
                    if (!strcmp(symbol, dummyBinding->variable)) {
                            return dummyBinding->value;
                    } else {
                            dummyBinding = dummyBinding->next;
                    }
            }
            return resolveVariable(symbol, env->nextEnv);
        }
        printf("Warning: There is no variable %s in this environment \n", symbol );
        return NULL;
}

Value *evalEach(Value *list, Environment *env){
    if (!list || list->type != consType) {
        printf("EvalEach bad argument\n");
        return NULL;
    }
    if (list->data.cons == NULL) { //no arguments
        return list;
    }
    Value *car = eval(list->data.cons->car, env);
    if (car == NULL) {
        return NULL;
    }
    Value *result;
    if (list->data.cons->cdr->data.cons) {
        result = evalEach(list->data.cons->cdr, env);
        if (result == NULL) {
            return NULL;
        }
            result->data.cons = addFirst(car, result->data.cons);
    } else {
        result = (Value *)malloc(sizeof(Value));
        result->type = consType;
        result->data.cons = addFirst(car, NULL);
    }
    return result;
}

Value *evalCar(Value *list, Environment *env) {
	if (!list || list->type != consType || list->data.cons == NULL || list->data.cons->cdr->data.cons != NULL) {
		printf("Warning: car takes exactly one argument\n");
		return NULL;
	}
	Value *result = eval(list->data.cons->car, env);
	if(result == NULL || result->type != consType || result->data.cons == NULL) {
		printf("Warning: must provide non-null list to car\n");
		return NULL;
	}
	return result->data.cons->car;
}

Value *evalCdr(Value *list, Environment *env) {
	if (!list || list->type != consType || list->data.cons == NULL || list->data.cons->cdr->data.cons != NULL) {
		printf("Warning: cdr takes exactly one argument\n");
		return NULL;
	}
	Value *result = eval(list->data.cons->car, env);
	if(result == NULL || result->type != consType || result->data.cons == NULL) {
		printf("Warning: must provide non-null list to car\n");
		return NULL;
	}
	return result->data.cons->cdr;
}

Value *evalBegin(Value *list, Environment *env){
    if (list == NULL || list->type != consType){
       printf("EvalBegin bad argument\n");
       return NULL;
    }
    if (list->data.cons == NULL) { //no arguments
        return makeVoidType();
    }
    Value *result = NULL;
    while(list->data.cons){
        result = eval(list->data.cons->car, env);
        list = list->data.cons->cdr;
    }
    return result;
}

Value *evalDefine(Value *list, Environment *env) {
    /* add the bindings to the top frame of the environment */
    if (list == NULL || list->type != consType || list->data.cons == NULL){
        printf("Warning: must provide arguments for define \n");
       return NULL;
    }
    if (list->data.cons->car->type == symbolType && list->data.cons->cdr->data.cons != NULL && list->data.cons->cdr->data.cons->cdr->data.cons == NULL) {
        Value *value = eval(list->data.cons->cdr->data.cons->car, env);
        bind(list->data.cons->car->data.symbolValue, value, env);
        return makeVoidType();
    } else {
        printf("Warning: need two elements and first must be a symbol\n");
        return NULL;
    }
}

Value *evalAnd(Value *list, Environment *env) {
    if (list == NULL || list->type != consType) {
        printf("Problem with and\n");
        return NULL;
    }
    if(list->data.cons == NULL) {
        Value *empty = (Value *)malloc(sizeof(Value));
        empty->type = boolType;
        empty->data.boolValue = 1;
        return empty;
    }
    Value *value = eval(list->data.cons->car, env);
    if (value == NULL || value->type != boolType) {
        printf("Warning: must have booleans in and statement\n");
        return NULL;
    }
    if (value->data.boolValue == 0) {
        return value;
    } else {
        return evalAnd(list->data.cons->cdr, env);
    }
}

Value *evalOr(Value *list, Environment *env) {
    if (list == NULL || list->type != consType) {
        printf("Problem with or\n");
        return NULL;
    }
    if(list->data.cons == NULL) {
        Value *empty = (Value *)malloc(sizeof(Value));
        empty->type = boolType;
        empty->data.boolValue = 0;
        return empty;
    }
    Value *value = eval(list->data.cons->car, env);
    if (value == NULL || value->type != boolType) {
        printf("Warning: must have booleans in or statement\n");
        return NULL;
    }
    if (value->data.boolValue == 1) {
        return value;
    } else {
        return evalOr(list->data.cons->cdr, env);
    }
}

Value *evalIf(Value *list, Environment *env) {
    if (list && list->type == consType && list->data.cons && list->data.cons->cdr->data.cons && (list->data.cons->cdr->data.cons->cdr->data.cons == NULL || list->data.cons->cdr->data.cons->cdr->data.cons->cdr->data.cons == NULL)) {
        Value *result = eval(list->data.cons->car, env);
        if (!result) {
            return NULL;
        }
        if((result->type == boolType) && (result->data.boolValue == 0)) {
            if (list->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
                return eval(list->data.cons->cdr->data.cons->cdr->data.cons->car, env);
            } else {
                printf("Warning: must have two arguments for a false statement\n");
                return NULL;
            }
        } else {
            return eval(list->data.cons->cdr->data.cons->car, env);
        }
    } else{
        printf("Warning: there must be at least two and no more than 3 args following the if \n");
        return NULL;
    }
}

Value *evalCond(Value *list, Environment *env) {
    if (!list || list->type != consType){
        return NULL;
    }
    if (list->data.cons == NULL) {
    	return makeVoidType();
    }
    if (list->data.cons->car->type != consType || list->data.cons->car->data.cons->cdr->data.cons == NULL || list->data.cons->car->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
        printf("Warning: there must be two arguments to the statements in cond\n");
        return NULL;
    }
    Value *condition = eval(list->data.cons->car->data.cons->car, env);
    if (condition && condition->type == boolType)
    {
        if (condition->data.boolValue == 1) {
            return eval(list->data.cons->car->data.cons->cdr->data.cons->car, env);
        }else{
            return evalCond(list->data.cons->cdr, env);
        }
    }
    return NULL;
}

Value *evalCons(Value *list, Environment *env) {
   if(!list || list->type != consType || list->data.cons == NULL || list->data.cons->car == NULL || list->data.cons->cdr->data.cons == NULL || list->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
        printf("Warning: must have exactly two arguments\n");
        return NULL;
    }
    Value *result = (Value *) malloc(sizeof(Value));
    result->type = consType;
    ConsCell *cell = (ConsCell *) malloc(sizeof(ConsCell));
    cell->car = eval(list->data.cons->car, env);
    if(cell->car == NULL) {
    	return NULL;
    }
    cell->cdr = eval(list->data.cons->cdr->data.cons->car, env);
    if(cell->cdr == NULL) {
    	return NULL;
    }
    result->data.cons = cell;
    return result;
}

Value *evalAppend(Value *list, Environment *env) {
	if(!list || list->type != consType || list->data.cons == NULL) {
		printf("Warning: must have at least argument and first must be a list\n");
		return NULL;
	}
	Value *value = appendHelper(list, env);
	if (value == NULL) {
		return NULL;
	}
	return value;
}

Value *appendHelper(Value *list, Environment *env) {
	Value *value = eval(list->data.cons->car, env);
	if(value) {
		if(list->data.cons->cdr->data.cons != NULL) { //it's not the last argument
			if (value->type == consType) { //makes sure the argument is a list and not a pair
				ConsCell *evalList = value->data.cons;
                if(evalList == NULL) { //if the argument is an empty list, return next argument
                    return appendHelper(list->data.cons->cdr, env);
                } else { //traverse the list to the item before null and connect it to the next list
                    //must check that this is not a pair while traversing the list
                    while(evalList->cdr->type == consType && evalList->cdr->data.cons != NULL) {
    					evalList = evalList->cdr->data.cons;
    				}
    				if (evalList->cdr->type == consType) {
                        Value *result = appendHelper(list->data.cons->cdr, env);
                        if (result != NULL) {
                            evalList->cdr=result;
                            return value;
                        } else {
                            return NULL;
                        }
                    } else {
                        printf("Warning: you have an argument that is not a list\n");
                        return NULL;
                    }
                }
			} else { //an argument in middle isn't a list
				printf("Warning: all arguments except last must be list\n");
				return NULL;
			}
		} else {
			return value;
		}
	}
	return NULL;
}

Value *evalLoad(Value *list, Environment *env) {
    if (!list || list->type != consType || list->data.cons->car->type != stringType || list->data.cons->cdr->data.cons != NULL)
    {
        printf("Warning: please enter exactly one string with quotes as the filename \n");
        return NULL;
    }
    int loaddepth = 0;
    char *filename = list->data.cons->car->data.stringValue;
    char buff[BUZZ_SIZE];
    FILE *f = fopen(filename, "r");

    if (f != NULL){
        ConsCell *loadTokens = NULL;
        ConsCell *loadParsetree = NULL;
        while (fgets(buff, BUZZ_SIZE, f)){
            loadTokens = tokenize(buff);
            loadTokens = reverse(loadTokens);

            // ignores when the user only inputs new lines or ;
            if (buff[0] != '\n' && buff[0] != ';') {
                if (!loadTokens) {
                    printf("Syntax Error\n");
                    deleteList(loadParsetree);
                    loadParsetree = NULL;
                }

                loadParsetree = parse(loadTokens, &loaddepth, loadParsetree);
                // after parseTree, tokens is undefined

                if (loaddepth < 0) {
                    printf("Syntax Error: too many close parentheses (start over!) \n");
                    loaddepth = 0;
                    deleteList(loadParsetree);
                    loadParsetree = NULL;
                }
                else if (loaddepth == 0) {
                    if (loadParsetree->car->type == consType && (loadParsetree->car->data.cons == NULL)) {
                        printConsCell(loadParsetree);
                        printf("\n");
                        loadParsetree = NULL;
                    }
                    else {
                        Value *result = eval(loadParsetree->car, env);
                        printResult(result);
                        loadParsetree = NULL;
                    }
                }
            }
        }
        return makeVoidType();
    } 
    else{
        printf("Warnings: There is no such file \"%s\" in the current directory \n", filename);
        return NULL;
    }
}

Value *evalDisplay(Value *list, Environment *env){
	if (!list || list->type != consType || list->data.cons == NULL || list->data.cons->cdr->data.cons != NULL) {
		printf("Warning: must provide exactly one argument for display\n");
        return NULL;
	}
	Value *curVal = eval(list->data.cons->car, env);
	if(curVal){
		switch(curVal->type) {
			case boolType:
				if(curVal->data.boolValue) {
                	printf("%s", "#t");
           		 } else {
                	printf("%s", "#f");
            	} 
            	return makeVoidType();
			case intType:
				printf("%d", curVal->data.intValue);
				return makeVoidType();
			case floatType:
				printf("%.2f", curVal->data.floatValue);
				return makeVoidType();
			case symbolType:
                printf("%s", curVal->data.symbolValue);
                return makeVoidType();
            case stringType:
            	printStringDisplay(curVal->data.stringValue);
				return makeVoidType();
			case consType:
                printf("(");
                printConsCell(curVal->data.cons);
                printf(")");
                return makeVoidType();
            case primitiveType:
                printf("Primitive Function");
                return makeVoidType();
            case closureType:
                printf("Closure Function");
                return makeVoidType();
            case voidType:
            	return makeVoidType();
            default:
				printf("Display Type: %d", curVal->type);
				return NULL;
		}
	}
	return NULL;
	
}

void printStringDisplay(char *stringVal){
	int i = 0;		
	while(i < strlen(stringVal)){
		if(stringVal[i] != 92){ //92 = '\'
			printf("%c", stringVal[i]);		
		} else {		
			if(stringVal[i+1] == 110){  //110 = 'n'
				printf("\n");
				i = i+1;
			}
			else if(stringVal[i+1] == 34){ // 34 = '"'
				printf("\"");
				i = i+1;
			}
			else if(stringVal[i+1] == 92){ // 92 = '\'
				printf("\\");
				i = i+1;
			}
			else{
				printf("Warning: invalid escape character");
				break;
			}
		}
		i = i+1;		
	}
}

//Evaluate the lambda expression and return a closure type value that contains the formals, body of the function, and environment pointing to enclosing environment
Value *evalLambda(Value *list, Environment *env){
    if (!list || list->type != consType || list->data.cons == NULL || list->data.cons->car->type != consType || list->data.cons->cdr->data.cons == NULL || list->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
        printf("Warning: lambda must have exactly two arguments and the first must be a list of inputs\n");
        return NULL;
    }
    Closure *resultClosure = (Closure *) malloc(sizeof(Closure));
    resultClosure->formals = list->data.cons->car;
    resultClosure->env = env;
    resultClosure->expr = list->data.cons->cdr->data.cons->car;
    Value *temp = (Value *) malloc(sizeof(Value));
    temp->type = closureType;
    temp->data.closureValue = resultClosure;
    return temp;
}

//Creates a new environment with the bindings specified in the list and then evaluates the body in this new environment. None of the bindings can see each other 

Value *evalLet(Value *list, Environment *env) {
    //list is consCell of consCell pairs to bind
    if (!list || list->type != consType || list->data.cons == NULL || list->data.cons->cdr->data.cons == NULL || list->data.cons->cdr->data.cons->cdr->data.cons != NULL){
       printf("Warning: must provide arguments to let including a list of variables to bind and exactly one statement to evaluate\n");
       return NULL;
    }
    ConsCell *pairList = list->data.cons->car->data.cons;
    Environment *newEnv = addFirstEnv(NULL, env);
    if (evalLetHelper(pairList, env, newEnv)) {
        //evaluate body
        Value *result = eval(list->data.cons->cdr->data.cons->car, newEnv);
        return result;
    }
    return NULL;
}

int evalLetHelper(ConsCell *pairList, Environment *oldEnv, Environment *newEnv) {
    if (pairList) {
        //error check
        if ((pairList->car->type != consType) || (pairList->car->data.cons == NULL)) {
            printf("Warning: there is no expression for binding\n");
            return 0;
        }
        ConsCell *pair = pairList->car->data.cons; //get first pair for binding
        if (pair->car->type != symbolType) {
            printf("Warning: you must specify a variable to bind\n");
            return 0;
        }
        if (pair->cdr->data.cons == NULL) {
            printf("Warning: you must specify a value to bind\n");
            return 0;
        }
        char *var = pair->car->data.symbolValue;
        Value *value = eval(pair->cdr->data.cons->car, oldEnv);
        if (value == NULL) {
            return 0;
        }
        bind(var, value, newEnv);
        return evalLetHelper(pairList->cdr->data.cons, oldEnv, newEnv);
    }
    return 1;
}

//Creates a new environment with the bindings specified in the list and then evaluates the body in this new environment. The bindings can see the bindings defined previously in the list
Value *evalLetStar(Value *list, Environment *env) {
    if (!list || list->type != consType || list->data.cons == NULL || list->data.cons->cdr->data.cons == NULL || list->data.cons->cdr->data.cons->cdr->data.cons != NULL){
       printf("Warning: must provide arguments to let* including a list of variables to bind and exactly one statement to evaluate\n");
       return NULL;
    }
    //list is consCell of consCell pairs to bind
    ConsCell *pairList = list->data.cons->car->data.cons;
    Environment *newEnv = evalLetStarHelper(pairList, env);
    if (newEnv != NULL) {
        //evaluate body
        Value *result = eval(list->data.cons->cdr->data.cons->car, newEnv);
        return result;
    }
    return NULL;
}

Environment *evalLetStarHelper(ConsCell *pairList, Environment *oldEnv) {
    if (pairList) {
        //error check
        if ((pairList->car->type != consType) || (pairList->car->data.cons == NULL)) {
            printf("Warning: there is no expression for binding\n");
            return NULL;
        }
        ConsCell *pair = pairList->car->data.cons;
        if (pair->car->type != symbolType) {
            printf("Warning: you must specify a variable to bind\n");
            return NULL;
        }
        if (pair->cdr->data.cons == NULL) {
            printf("Warning: you must specify a value to bind\n");
            return NULL;
        }
        char *var = pair->car->data.symbolValue;
        Value *value = eval(pair->cdr->data.cons->car, oldEnv);
        if (value == NULL) {
            return NULL;
        }
        //must create a new environment here so we can make a chain like structure of environments so that the example

        //(let* ( (z 5) (temp (lambda (x) (+ x y))) (y 5)) (temp 3))

        //does not work

        Environment *newEnv = addFirstEnv(NULL, oldEnv);
        bind(var, value, newEnv);
        return evalLetStarHelper(pairList->cdr->data.cons, newEnv);
    }
    return oldEnv;
}

//implemented letrec based on what it does in dr racket which seems like

    //(letrec ((var expr)...) body)

    //(let ((var #<undefined>) ...)
    //    (set! var expr) ...
    //    (let () body)
Value *evalLetRec(Value *list, Environment *env) {
    if (!list || list->type != consType || list->data.cons == NULL || list->data.cons->cdr->data.cons == NULL || list->data.cons->cdr->data.cons->cdr->data.cons != NULL){
       printf("Warning: must provide arguments to letRec including a list of variables to bind and exactly one statement to evaluate\n");
       return NULL;
    }
    ConsCell *pairList = list->data.cons->car->data.cons;
    Environment *envOne = addFirstEnv(NULL, env);
    //create a new environment of the outer let bindings
    if(letRecHelper(pairList, envOne)) {
        while(pairList) { //set! each pair
            if (evalSetBang(pairList->car, envOne) == NULL) { 
            	return NULL;
            }
            pairList = pairList->cdr->data.cons;
        }
        Environment *envTwo = addFirstEnv(NULL, envOne);
        Value *result = eval(list->data.cons->cdr->data.cons->car, envTwo);
        return result;
    }
    return NULL;
}

int letRecHelper(ConsCell *pairList, Environment *newEnv) {
    if (pairList) {
        //error check
        if ((pairList->car->type != consType) || (pairList->car->data.cons == NULL)) {
            printf("Warning: there is no expression for binding\n");
            return 0;
        }
        ConsCell *pair = pairList->car->data.cons;
        if (pair->car->type != symbolType) {
            printf("Warning: you must specify a variable to bind\n");
            return 0;
        }
        if (pair->cdr->data.cons == NULL) {
            printf("Warning: you must specify a value to bind\n");
            return 0;
        }
        char *var = pair->car->data.symbolValue;
        Value *value = (Value *) malloc(sizeof(Value)); 
        value->type = boolType;
        value->data.boolValue = 0;
        bind(var, value, newEnv);
        return letRecHelper(pairList->cdr->data.cons, newEnv);
    }
    return 1;
}

//finds if the variable to redefine exists in the scope, and if it does, it redefines it
Value *evalSetBang(Value *list, Environment *env) { 
    if (!list || list->type != consType || list->data.cons == NULL || list->data.cons->car->type != symbolType || list->data.cons->cdr->data.cons == NULL || list->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
        printf("Warning: must provide exactly two arguments (symbol and value) to set!\n");
        return NULL;
    }
    char *variable = list->data.cons->car->data.symbolValue;
    Value *value = eval(list->data.cons->cdr->data.cons->car, env);
    Binding *binding = NULL;
    while (env) {
        binding = env->bindingHead;
        while (binding) {
            if (!strcmp(variable, binding->variable)) {
                binding->value = value;
                return makeVoidType();
            } else {
                binding = binding->next;
            }
        }
        env = env->nextEnv;
    }
    printf("Warning: %s was not previously defined\n", variable);
    return NULL;
}

void printResult(Value *value) {
	if (value) {	
		switch(value->type) {
			case boolType: {
                if(value->data.boolValue) {
                    printf("%s\n", "#t");
                }
                else {
                    printf("%s\n", "#f");
                } 
                break;
            }
            case intType:
				printf("%d\n", value->data.intValue);
				break;
			case floatType:
				printf("%.2f\n", value->data.floatValue);
				break;
			case symbolType:
                printf("%s\n", value->data.symbolValue);
                break;
            case stringType:
				printf("\"%s\"\n", value->data.stringValue);
				break;
			case consType:
                printf("(");
                printConsCell(value->data.cons);
                printf(")\n");
                break;
            case primitiveType:
                printf("Primitive Function\n");
                break;
            case closureType:
                printf("Closure Function\n");
                break;
            case voidType:
            	printf("\n");
                break;
            default:
				printf("Type: %d\n", value->type);
				break;
		}
	}
}


