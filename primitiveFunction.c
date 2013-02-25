#include "header.h"

/* Make values to return*/
Value *makeIntValue(int integer){
	Value *result = (Value *) malloc(sizeof(Value));
	result->type = intType;
	result->data.intValue = integer;
	return result;
}

Value *makeFloatValue(float flt){
	Value *result = (Value *) malloc(sizeof(Value));
	result->type = floatType;
	result->data.floatValue = flt;
	return result;
}

Value *makeBoolValue(int TrueOrFalse){
	Value *result = (Value *) malloc(sizeof(Value));
	result->type = boolType;
	result->data.boolValue = TrueOrFalse;
	return result;
}

Value *add(Value *args) {
	if(args && args->type == consType) {
		return addHelper(args);
	} else {
		return NULL;
	}
}

Value *addHelper(Value *args){
	if (args->data.cons == NULL) {
		return makeIntValue(0);
	} else {
		switch(args->data.cons->car->type) {
			case intType: {
				//There might be some memory issues here
				Value *recur = addHelper(args->data.cons->cdr);
				if (recur->type == floatType) {
					float result = args->data.cons->car->data.intValue + recur->data.floatValue;
					free(recur);
					return makeFloatValue(result);
				}
				int result = args->data.cons->car->data.intValue + recur->data.intValue;
				free(recur);
				return makeIntValue(result);
			}
			case floatType: {
				//There might be some memory issues here
				Value *recur = addHelper(args->data.cons->cdr);
				float result;
				if (recur->type == floatType){
            		result = args->data.cons->car->data.floatValue + recur->data.floatValue;
				}else{
					result = args->data.cons->car->data.floatValue + recur->data.intValue;
				}
				
				free(recur);
				return makeFloatValue(result);
			}
			default: {
				printf("Warning: non integer/float input to add function \n");
				return NULL;
			}
		}
	}
}

Value *subtract(Value *args) {
	if(args) {
		return subtractHelper(args);
	} else {
		return NULL;
	}
}

Value *subtractHelper(Value *args){
	if (args->data.cons == NULL)
	{
		return makeIntValue(0);
	} else {
		switch(args->data.cons->car->type) {
			case intType: {
				//There might be some memory issues here
				Value *recur = addHelper(args->data.cons->cdr);
				if (recur->type == floatType) {
					float result = args->data.cons->car->data.intValue - recur->data.floatValue;
					free(recur);
					return makeFloatValue(result);
				}
				int result = args->data.cons->car->data.intValue - recur->data.intValue;
				free(recur);
				return makeIntValue(result);
			}
			case floatType: { 
				Value *recur = addHelper(args->data.cons->cdr);
				float result;
				if (recur->type == floatType){
            		result = args->data.cons->car->data.floatValue - recur->data.floatValue;
				}else{
					result = args->data.cons->car->data.floatValue - recur->data.intValue;
				}
				
				free(recur);
				return makeFloatValue(result);
			}
			default: {
				printf("Warning: non integer/float input to add function \n");
				return NULL;
			}
		}
	}
}

Value *division(Value *args) {
	if(args) {
		return divisionHelper(args);
	} else {
		return NULL;
	}
}

Value *divisionHelper(Value *args){
	if (args->data.cons == NULL)
	{
		return makeIntValue(1);
	} else {
		switch(args->data.cons->car->type) {
			case intType: {
				//There might be some memory issues here
				Value *recur = multiplyHelper(args->data.cons->cdr);
				if (recur->type == floatType) {
					if (recur->data.floatValue == 0.00)
					{
						printf("Warning: division by zero \n");
						return NULL;
					}
					float result = args->data.cons->car->data.intValue / recur->data.floatValue;
					free(recur);
					return makeFloatValue(result);
				}
				if (recur->data.intValue == 0)
				{
					printf("Warning: division by zero \n");
					return NULL;
				}
				float result = args->data.cons->car->data.intValue / ((float)recur->data.intValue);
				free(recur);
				//checks if the result should be a float or int
				if (ceil(result) == result) {
					return makeIntValue(result);
				} else {
					return makeFloatValue(result);
				}
			}
			case floatType: {
				// There might be some memory issues here
				Value *recur = multiplyHelper(args->data.cons->cdr);
				float result;
				if (recur->type == floatType){
					if (recur->data.floatValue== 0.00)
					{
						printf("Warning: division by zero \n");
						return NULL;
					}
            		result = args->data.cons->car->data.floatValue / recur->data.floatValue;
				}else{
					if (recur->data.intValue == 0)
						{
							printf("Warning: division by zero \n");
							return NULL;
						}
					result = args->data.cons->car->data.floatValue / recur->data.intValue;
				}
				
				free(recur);
				return makeFloatValue(result);
			}
			default: {
				printf("Warning: non integer/float input to multiply function \n");
				return NULL;
			}
		}
	}
}

Value *multiply(Value *args) {
	if(args) {
		return multiplyHelper(args);
	} else {
		return NULL;
	}
}

Value *multiplyHelper(Value *args){
	if (args->data.cons == NULL)
	{
		return makeIntValue(1);
	} else {
		switch(args->data.cons->car->type) {
			case intType: {
				// There might be some memory issues here
				Value *recur = multiplyHelper(args->data.cons->cdr);
				if (recur->type == floatType) {
					float result = args->data.cons->car->data.intValue * recur->data.floatValue;
					free(recur);
					return makeFloatValue(result);
				}
				int result = args->data.cons->car->data.intValue * recur->data.intValue;
				free(recur);
				return makeIntValue(result);
			}
			case floatType: {
				// There might be some memory issues here
				Value *recur = multiplyHelper(args->data.cons->cdr);
				float result;
				if (recur->type == floatType){
            		result = args->data.cons->car->data.floatValue * recur->data.floatValue;
				}else{
					result = args->data.cons->car->data.floatValue * recur->data.intValue;
				}
				free(recur);
				return makeFloatValue(result);
			}
			default: {
				printf("Warning: non integer/float input to multiply function \n");
				return NULL;
			}
		}
	}
}

Value *isOdd(Value *cell){
	if (!cell || cell->type != consType || cell->data.cons == NULL || cell->data.cons->cdr->data.cons != NULL) {
		printf("Warning: odd? must take one argument\n");
		return NULL;
	}
	if (cell->data.cons->car->type == intType)
	{
		if (cell->data.cons->car->data.intValue %2 == 0)
		{
			return makeBoolValue(0);
		}else{
			return makeBoolValue(1);
		}
	}else{
		printf("Warning: the number is not odd, but it is not a integer \n" );
		return NULL;
	}
}

Value *isEven(Value *cell){
	if (!cell || cell->type != consType || cell->data.cons == NULL || cell->data.cons->cdr->data.cons != NULL) {
		printf("Warning: even? must take one argument\n");
		return NULL;
	}
	if (cell->data.cons->car->type == intType)
	{
		if (cell->data.cons->car->data.intValue %2 == 0)
		{
			return makeBoolValue(1);
		}else{
			return makeBoolValue(0);
		}
	}else{
		printf("Warning: the number is not even, but it is not a integer \n" );
		return NULL;
	}
}

Value *isZero(Value *cell){
	if (!cell || cell->type != consType || cell->data.cons == NULL || cell->data.cons->cdr->data.cons != NULL) {
		printf("Warning: zero? must take one argument\n");
		return NULL;
	}
	switch (cell->data.cons->car->type){
		case floatType: 
			if (cell->data.cons->car->data.floatValue == 0.00)
			{
				return makeBoolValue(1);
			}else{
				return makeBoolValue(0);
			}
		case intType: 
			if (cell->data.cons->car->data.intValue == 0)
			{
				return makeBoolValue(1);
			}else{
				return makeBoolValue(0);
			}
		default: 
			printf("Warning: arg must be a number \n");
			return NULL;
	}
}

Value *isNull(Value *args){
	// assert that there is exactly one argument and it's a list
  	if (!args || args->type != consType || args->data.cons == NULL || args->data.cons->car->type != consType || args->data.cons->cdr->data.cons != NULL) {
		printf("Warning: null? must take one list argument\n");
		return NULL;
	}
	int result = (args->data.cons->car->data.cons == NULL);
	return makeBoolValue(result);
}

Value *isGreaterOrEqual(Value *args){
	// Check if the arguments has two values
	if (!args || args->type != consType || args->data.cons == NULL || args->data.cons->cdr->data.cons == NULL || args->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
		printf("Warning: please enter exactly two ints for equal\n");
		return NULL;
	}
	Value *first = args->data.cons->car;
	Value *second = args->data.cons->cdr->data.cons->car; 
	if (first->type != intType && first->type != floatType)
	{
		printf("Warning: first argument is not a number \n");
		return NULL;
	}
	if (second->type != intType && second->type != floatType)
	{
		printf("Warning: second argument is not a number \n");
		return NULL;
	}
	switch (first->type){
		case intType: 
			if (second->type == intType)
			{
				return makeBoolValue(first->data.intValue >=  second->data.intValue);
			}else{
				return makeBoolValue(first->data.intValue >=  second->data.floatValue);
			}
		case floatType: 
			if (second->type == intType)
			{
				return makeBoolValue(first->data.floatValue >=  second->data.intValue);
			}else{
				return makeBoolValue(first->data.floatValue >=  second->data.floatValue);
			}
	}
}
Value *isGreater(Value *args){
	//Check if the arguments has two values
	if (!args || args->type != consType || args->data.cons == NULL || args->data.cons->cdr->data.cons == NULL || args->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
		printf("Warning: please enter exactly two ints for equal\n");
		return NULL;
	}
	Value *first = args->data.cons->car;
	Value *second = args->data.cons->cdr->data.cons->car; 
	if (first->type != intType && first->type != floatType)
	{
		printf("Warning: first argument is not a number \n");
		return NULL;
	}
	if (second->type != intType && second->type != floatType)
	{
		printf("Warning: second argument is not a number \n");
		return NULL;
	}
	switch (first->type){
		case intType: 
			if (second->type == intType)
			{
				return makeBoolValue(first->data.intValue >  second->data.intValue);
			}else{
				return makeBoolValue(first->data.intValue >  second->data.floatValue);
			}
		case floatType: 
			if (second->type == intType)
			{
				return makeBoolValue(first->data.floatValue >  second->data.intValue);
			}else{
				return makeBoolValue(first->data.floatValue >  second->data.floatValue);
			}
	}
}

Value *isLessOrEqual(Value *args){
	//Check if the arguments has two values
	if (!args || args->type != consType || args->data.cons == NULL || args->data.cons->cdr->data.cons == NULL || args->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
		printf("Warning: please enter exactly two ints for equal\n");
		return NULL;
	}
	Value *first = args->data.cons->car;
	Value *second = args->data.cons->cdr->data.cons->car;  
	if (first->type != intType && first->type != floatType)
	{
		printf("Warning: first argument is not a number \n");
		return NULL;
	}
	if (second->type != intType && second->type != floatType)
	{
		printf("Warning: second argument is not a number \n");
		return NULL;
	}
	switch (first->type){
		case intType: 
			if (second->type == intType)
			{
				
				return makeBoolValue(first->data.intValue <=  second->data.intValue);
			}else{
				return makeBoolValue(first->data.intValue <=  second->data.floatValue);
			}
		case floatType: 
			if (second->type == intType)
			{
				return makeBoolValue(first->data.floatValue <=  second->data.intValue);
			}else{
				return makeBoolValue(first->data.floatValue <=  second->data.floatValue);
			}
	}
}

Value *isLess(Value *args){
	//Check if the arguments has two values
	if (!args || args->type != consType || args->data.cons == NULL || args->data.cons->cdr->data.cons == NULL || args->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
		printf("Warning: please enter exactly two ints for equal\n");
		return NULL;
	}
	Value *first = args->data.cons->car;
	Value *second = args->data.cons->cdr->data.cons->car; 
	if (first->type != intType && first->type != floatType)
	{
		printf("Warning: first argument is not a number \n");
		return NULL;
	}
	if (second->type != intType && second->type != floatType)
	{
		printf("Warning: second argument is not a number \n");
		return NULL;
	}
	switch (first->type){
		case intType: 
			if (second->type == intType)
			{
				return makeBoolValue(first->data.intValue <  second->data.intValue);
			}else{
				return makeBoolValue(first->data.intValue <  second->data.floatValue);
			}
		case floatType: 
			if (second->type == intType)
			{
				return makeBoolValue(first->data.floatValue <  second->data.intValue);
			}else{
				return makeBoolValue(first->data.floatValue <  second->data.floatValue);
			}
	}
}

Value *numEqual(Value *args){
	//Check if the arguments has two values
	if (!args || args->type != consType || args->data.cons == NULL || args->data.cons->cdr->data.cons == NULL || args->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
		printf("Warning: please enter exactly two ints for equal\n");
		return NULL;
	}
	Value *first = args->data.cons->car;
	Value *second = args->data.cons->cdr->data.cons->car; 
	if (first->type != intType && first->type != floatType)
	{
		printf("Warning: first argument is not a number \n");
		return NULL;
	}
	if (second->type != intType && second->type != floatType)
	{
		printf("Warning: second argument is not a number \n");
		return NULL;
	}
	switch (first->type){
		case intType: 
			if (second->type == intType)
			{
				return makeBoolValue(first->data.intValue ==  second->data.intValue);
			}else{
				return makeBoolValue(first->data.intValue ==  second->data.floatValue);
			}
		case floatType: 
			if (second->type == intType)
			{
				return makeBoolValue(first->data.floatValue ==  second->data.intValue);
			}else{
				return makeBoolValue(first->data.floatValue ==  second->data.floatValue);
			}
	}
}

Value *isInt(Value *arg){
	if (!arg || arg->type != consType || arg->data.cons == NULL || arg->data.cons->cdr->data.cons != NULL) {
		printf("Warning: integer? must take one argument\n");
		return NULL;
	}
	if (arg->data.cons->car->type == intType)
	{
		return makeBoolValue(1);
	}else{
		return makeBoolValue(0);
	}
}

Value *isPair(Value *arg){
	if (!arg || arg->type != consType || arg->data.cons == NULL || arg->data.cons->cdr->data.cons != NULL) {
		printf("Warning: pair? must take one argument\n");
		return NULL;
	}
	if (arg->data.cons->car->type == consType)
	{
		return makeBoolValue(1);
	}else{
		return makeBoolValue(0);
	}

}

Value *modulo(Value *args){
	if (!args || args->type != consType || args->data.cons == NULL || args->data.cons->car->type != intType || args->data.cons->cdr->data.cons == NULL || args->data.cons->cdr->data.cons->car->type != intType || args->data.cons->cdr->data.cons->cdr->data.cons != NULL) {
		printf("Warning: please enter exactly two ints for modulo\n");
		return NULL;
	}
	int first = args->data.cons->car->data.intValue;
	int second = args->data.cons->cdr->data.cons->car->data.intValue;
	if(first < second){
		return makeIntValue(first);
	}
	int divResult = first / second; //this will be an int result
    return makeIntValue(first - (divResult*second));
}

Value *not(Value *arg){
	if (!arg || arg->type != consType || arg->data.cons == NULL || arg->data.cons->cdr->data.cons != NULL) {
		printf("Warning: not must take one argument\n");
		return NULL;
	}
	if (arg->data.cons->car->type == boolType)
	{
		return makeBoolValue(1-arg->data.cons->car->data.boolValue);
	}else{
		return makeBoolValue(0);
	}
}
