#include "header.h"

Value *makeVoidType(){
    Value *new = (Value*) malloc(sizeof(Value));
    new->type = voidType;
    return new;
}

ConsCell *addFirst(Value *car, ConsCell *old) {
    ConsCell *new = (ConsCell *) malloc(sizeof(ConsCell));
    new->car = car;
    Value *temp = (Value *) malloc(sizeof(Value));
    temp->type = consType;
    temp->data.cons = old;
    new->cdr = temp;
    return new;
}

ConsCell *pop(ConsCell *old, Value **value){
    ConsCell *next = old->cdr->data.cons;
    *value = old->car;
    free(old->cdr);
    free(old);
    return next;
}

Value *getLast(ConsCell *cell) {
    if (cell->cdr->data.cons == NULL) { //at end of list
        return cell->car;
    } else {
        return getLast(cell->cdr->data.cons);
    }

}

void deleteValue(Value *value){
    if(value->type == stringType) {
        free(value->data.stringValue);
    }
    else if (value->type == symbolType) {
        free(value->data.symbolValue);
    }
    else if (value->type == consType) {
        deleteList(value->data.cons);
    }
    free(value);
}

void deleteList(ConsCell *cell){
	if (cell != NULL){
        deleteList(cell->cdr->data.cons);
        free(cell->cdr);
        deleteValue(cell->car);
        free(cell);
    }
}

int getCount(ConsCell *cell, int num){
    if(cell == NULL){
		return num;
	}
	else {
		num += 1;
		return getCount(cell->cdr->data.cons, num);
	}
}

ConsCell *reverseHelper(ConsCell *cell, ConsCell *list){
	//printf("%d\n", getLast(cell)->car->data.intValue);
	if (cell != NULL){
	    Value *temp = (Value *) malloc(sizeof(Value));
        temp->type = cell->car->type;
        if(temp->type == boolType) {
            temp->data.boolValue = cell->car->data.boolValue;
        }
        else if(temp->type == intType) {
            temp->data.intValue = cell->car->data.intValue;
        }
        else if(temp->type == floatType) {
            temp->data.floatValue = cell->car->data.floatValue;
        }
        else if (temp->type == stringType) {
            temp->data.stringValue = (char *) malloc((strlen(cell->car->data.stringValue) + 1)*sizeof(char));
            strcpy(temp->data.stringValue, cell->car->data.stringValue);
        }
        else if (temp->type == symbolType) {
            temp->data.symbolValue = (char *) malloc((strlen(cell->car->data.symbolValue) + 1)*sizeof(char));
            strcpy(temp->data.symbolValue, cell->car->data.symbolValue);
        }
	    list = addFirst(temp,list);
	    return reverseHelper(cell->cdr->data.cons, list);
	}
	else {
	    return list;
	}
}

ConsCell *reverse(ConsCell *cell){
	ConsCell *list = NULL;
    if (cell) { /* makes sure cell is not NULL */
        list = reverseHelper(cell, list);
        deleteList(cell);
    }
	return list;
}

void printValue(Value *value){
    if (value != NULL) {
        int type = value->type;
        if(type == boolType) {
            if(value->data.boolValue) {
                printf("%s", "#t");
            }
            else {
                printf("%s", "#f");
            } 
        }
        else if(type == intType) {
            printf("%d", value->data.intValue);
        }
        else if(type == floatType) {
            printf("%f", value->data.floatValue);
        }
        else if(type == stringType) {
            printf("\"%s\"", value->data.stringValue);
        }
        else if(type == symbolType) {
            printf("%s", value->data.symbolValue);
        }
        else if (type == consType){ //type == consType
            printf("(");
            printConsCell(value->data.cons);
            printf(")");
        }
        else if (type == closureType){
            printf("Closure Function");
        }
        else if (type == primitiveType) {
            printf("Primitive Function");
        }
        else if (type == voidType) {
        	printf("Void Type");
        }
        else {
            printf("Error: type %d undefined\n", type);
        }
    }
}

void printConsCell(ConsCell *cell){
    /* print the whole parseTree */
    if (cell != NULL) {
        printValue(cell->car);
        if(cell->cdr) {
        	if (cell->cdr->type == consType){
            	if(cell->cdr->data.cons) {
            		printf(" ");
            		printConsCell(cell->cdr->data.cons);
            	}
            } else {
            	printf(" . ");
            	printValue(cell->cdr);
            }
        }
        
    }
}
