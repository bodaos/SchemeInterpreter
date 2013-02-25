#include "header.h"

ConsCell *parse(ConsCell *tokens, int *depth, ConsCell *tree){
	/* Parse the tokens*/
	Value *val = NULL;
	while(tokens) {
		/* take first cell of tokens and push in onto our stack */
		tokens = pop(tokens, &val);
		tree = addFirst(val, tree);
		if (val->type == openType) {
			*depth += 1;
		}
		else if (val->type == closeType){
			*depth -= 1;
			if (*depth < 0) { /*too many close parens*/
				if (tokens) {
					deleteList(tokens); /* free rest of tokens */
				}
				return tree;
			}
			Value *temp = NULL;
			ConsCell *new = NULL;
			/* pop off close paren because we don't won't it in our parseTree */
			tree = pop(tree, &temp);
			free(temp);
			/* pop off cells from stack until we find the matching open paren */
			while (tree->car->type != openType) {
				tree = pop(tree, &temp);
				new = addFirst(temp, new);
			}
			/* remove open paren */
			tree = pop(tree, &temp);
			free(temp);
			/* create a new cons cell to hold the list */
			Value *newVal = (Value *) malloc(sizeof(Value));
			newVal->type = consType;
			newVal->data.cons = new;
			/* push new cons cell to stack */
			tree = addFirst(newVal, tree);
		}
	}
	return tree;
}