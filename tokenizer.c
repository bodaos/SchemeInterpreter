#include "header.h"


ConsCell *tokenize(char *expression){
    ConsCell *cell = NULL;
    /* use start to mark the string that will become the value of the token;
    use memmove to move the characters from expression to value */
    int start = 0;
    int inString = 0;
    int inSym = 0;
    int inNum = 0;
    int inBool =0;
    int hasDecimal = 0;
    int type = -1;

    char ch;

    int i;
    for(i = 0; i < 256; i++){
        ch = expression[i];
        //printf("%d : %d ", i, ch);
        if (ch == '\"' && !escapedQuote(expression, i)) { /* checking if " is escaped or if at beginning */
            if(!inString) {
                /* check if need to create token */
                if (start != i) {
                    cell = addToken(cell, type, expression, start, i);
                    inString = 0;
                    inSym = 0;
                    inNum = 0;
                    inBool = 0;
                    hasDecimal = 0;
                    type = -1;
                }
                inString = 1;
            }
            else {
                inString = 0;
                /* generate a string */
                cell = addToken(cell, stringType, expression, start, i);
                inString = 0;
                inSym = 0;
                inNum = 0;
                inBool = 0;
                hasDecimal = 0;
                type = -1;
            }
            start = i + 1; 
        }
        else if (!inString) {
            if (ch == '('){
                /* check if need to create token */
                if (start != i) {
                    cell = addToken(cell, type, expression, start, i);
                    inString = 0;
                    inSym = 0;
                    inNum = 0;
                    inBool = 0;
                    hasDecimal = 0;
                    type = -1;
                }
                cell = addToken(cell, openType, expression, i, i);
                inString = 0;
                inSym = 0;
                inNum = 0;
                inBool = 0;
                hasDecimal = 0;
                type = -1;
                start = i + 1; /* the next character could be the start */
            }
            else if (ch == ')') {
                /* check if need to create token */
                if (start != i) {
                    cell = addToken(cell, type, expression, start, i);
                    inString = 0;
                    inSym = 0;
                    inNum = 0;
                    inBool = 0;
                    hasDecimal = 0;
                    type = -1;
                }
                cell = addToken(cell, closeType, expression, i, i);
                inString = 0;
                inSym = 0;
                inNum = 0;
                inBool = 0;
                hasDecimal = 0;
                type = -1;
                start = i + 1; /* the next character could be the start */
            }
            else if(ch == ' ' || ch == '\t') {
                /* see if this has ended a symbol/number */
                if (start != i) { /* symbol/number just ended */
                    cell = addToken(cell, type, expression, start, i);
                    inString = 0;
                    inSym = 0;
                    inNum = 0;
                    inBool = 0;
                    hasDecimal = 0;
                    type = -1;
                }
                start = i + 1; /* the next character could be the start */
            }
            /*check if start of num*/
            else if(isdigit(ch) && !inSym) {
                   if (!inNum) { /* it is not in number */                      
                        if (start == i) {
                            inNum = 1;
                            type = intType; /* set the default value to be int, if we encounter decimal then we change it to float */
                        }
                        else {
                            printf("error at %d: this digit is not in number and not in string and not in symbol, where can this be? \n", i);
                            deleteList(cell);
                            return NULL;
                        }
                   }
                
            }
            /*check if it is a decimal place */
            else if (ch == '.' && !inSym){
                if (start == i) { /*if the . is the start of a number */
                    if (isdigit(expression[i+1]))
                    {
                        inNum = 1;
                        type = floatType;
                        hasDecimal = 1;
                    } else {
                        printf("error at %d: . not followed by a digit as a initial \n", i);
                        deleteList(cell);
                        return NULL;
                    }
                }
                else { /*if . is not the start of a number */
                    if (hasDecimal) {
                        printf("error at %d: there are two decimals \n", i);
                        deleteList(cell);
                        return NULL;
                    }
                    if (!inNum) {
                        printf("error at %d: . not in number or string or symbol, where can it be? \n", i);
                        deleteList(cell);
                        return NULL;
                    }
                    type = floatType;
                    hasDecimal = 1;
                }
            }
            else if ((ch == 'f' || ch == 't') && inBool)
            {
                if (isTerminator(expression[i+1])){
                    type = boolType;
                }
                else {
                    printf("error at %d: # not allowed in non-Boolean \n", i);
                    deleteList(cell);
                    return NULL;
                }
            }
            /*check if start of sym*/
            else if(isInitial(ch)) {
                if (!inNum) {
                    inSym = 1;
                    type = symbolType;
                }
                else {
                    printf("error at %d: can't have numbers starting a symbol\n", i);
                    deleteList(cell);
                    return NULL;
                } 
            }
            /*deal with + or 1*/
            else if(!inSym && ((ch == '+') || (ch == '-'))) {
                if (isdigit(expression[i+1]) || expression[i+1] == '.'){
                    inNum = 1;
                    type = intType; /* set the default value to be int, if we encounter decimal then we change it to float */
                }
                else if (isTerminator(expression[i+1])){
                    inSym = 1;
                    type = symbolType;
                }
                else {
                    printf("error at %d: -/+ can't start a symbol (other than the single character +/-)\n", i);
                    deleteList(cell);
                    return NULL;
                }
            }
            else if (ch == '#'){
                if (start != i){ /*checks that we are not in a number or symbol */
                    printf("error at %d: # not allowed in symbol or number \n", i); 
                    deleteList(cell);
                    return NULL;
                } else if (expression[i+1] != 'f' && expression[i+1] != 't') {
                   printf("error at %d: # not followed by t or f \n", i);
                   deleteList(cell);
                   return NULL;
                }
                else {
                    inBool = 1;
                }
            }
            else if(isSubsequent(ch)) {
                continue;
            }
            else if (((ch == ';') || (ch == '\n') || (ch == '\0')) && !inString) {
                /* see if this has ended a symbol/number */
                if (start != i) { /* symbol/number just ended */
                    cell = addToken(cell, type, expression, start, i);
                    inString = 0;
                    inSym = 0;
                    inNum = 0;
                    inBool = 0;
                    hasDecimal = 0;
                    type = -1;
                }
                /* end loop */
                break;
            }
            else {
                printf("There was an error with syntax at character %d which is %c\n", i, ch);
                deleteList(cell);
                return NULL;
            }
        }
    }
    /*check if a string wasn't closed*/
    if (inString) {
        printf("There was not ending \"\n");
        deleteList(cell);
        return NULL;
    }
    return cell;
}

ConsCell *addToken(ConsCell *cell, int type, char *expression, int start, int end){
    char *temp;
    Value *val = (Value *) malloc(sizeof(Value));
    val->type = type;

    //printf("Value: %d, Start: %d, End: %d, Expression: %s\n", type, start, end, expression);

    if(type == boolType) {
        if(expression[end - 1] == 'f'){
            val->data.boolValue = 0;
        }
        else {
            val->data.boolValue = 1;
        }
    }
    else if(type == intType) {
        temp = (char *) malloc((end - start + 1)*sizeof(char));
        strncpy(temp, expression + start, end - start);
        temp[end - start] = '\0';
        val->data.intValue = atoi(expression + start);
        free(temp);
    }
    else if(type == floatType){
        temp = (char *) malloc((end - start + 1)*sizeof(char));
        strncpy(temp, expression + start, end - start);
        temp[end - start] = '\0';
        val->data.floatValue = atof(temp);
        free(temp);
    }
    else if(type == stringType) {
        val->data.stringValue = (char *) malloc((end - start + 1)*sizeof(char));
        strncpy(val->data.stringValue, expression + start, end - start);
        val->data.stringValue[end - start] = '\0';
    }
    else if(type == symbolType) {
        val->data.symbolValue = (char *) malloc((end - start + 1)*sizeof(char));
        strncpy(val->data.symbolValue, expression + start, end - start);
        val->data.symbolValue[end - start] = '\0';
    }
    /* if not the 5 types above, must be a open or close paren*/
    return addFirst(val, cell);
}

int isInitial(char c) {
    if (isalpha(c) || (c == '!') || (c == '$') || (c == '%') || (c == '&') || (c == '*') || (c == '/') || (c == ':') || (c == '<') || (c == '=') || (c == '>') || (c == '?') || (c == '~') || (c == '_') || (c == '^')){
        return 1;
    }
    return 0;

}

int isSubsequent(char c) {
    if (isInitial(c) || isdigit(c) || (c == '.') || (c == '+') || (c == '-')){
        return 1;
    }
    return 0;
}

int isTerminator(char c) {
    if (c == ' ' || c == '\"' || c == '(' || c == ')' || c == '\n' || c == ';' || c == '\0') {
        return 1;
    }
    return 0;
}

/* returns 1 if the string is escaped (has an odd number of \ before it) and 0 otherwise */
int escapedQuote(char *expr, int c) {
	int count = 0;
	int i = c-1;
	for(i; i >= 0; i--) {
		if(expr[i] == '\\') {
			count++;
		} else {
			break;
		}
	}
	if ((count % 2) == 0) {
		return 0;
	}
	return 1;
}






