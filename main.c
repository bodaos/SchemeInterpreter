#include "header.h"

/*valgrind --dsymutil=yes --leak-check=full*/

/*LINKED LIST MAIN
------------------------------------------*/
/*int main(int argc, char *argv[]) {
    ConsCell *list = NULL;
    Value *val;
    int i;
    for (i = 1; i <= 5; i++) {
        val = (Value *) malloc(sizeof(Value));
        val->type = intType;
        val->data.intValue = i;
        list = addFirst(val, list);
    }
    val = (Value *) malloc(sizeof(Value));
    val->type = stringType;
    val->data.stringValue = "Hello";
    list = addFirst(val, list);
    printList(list);
    while (list){
        list = pop(list, &val);
        printValue(val);
        free(val);
    }
    return 0;
}*/

/*TOKENIZER MAIN
------------------------------------------*/
/*int main(int argc, char *argv[]) {
    ConsCell *tokens;
    char *expression = malloc(256 * sizeof(char));
    while (fgets(expression, 256, stdin) && (*expression != 'q')) {
        tokens = tokenize(expression);
        tokens = reverse(tokens);
        printList(tokens);
        deleteList(tokens);
    }
    free(expression);
    return 0;
}*/

/*PARSER MAIN
------------------------------------------*/
/*
int main(int argc, char *argv[]) {
    ConsCell *tokens = NULL, *parseTree = NULL;
    int depth = 0;
    char *expression = malloc(256 * sizeof(char));

    while (fgets(expression, 256, stdin) && (*expression != 'q')) {

        tokens = tokenize(expression);
        tokens = reverse(tokens);

        // ignores when the user only inputs new lines
        if (expression[0] != '\n') {
            if (!tokens) {
                printf("Syntax Error\n");
                deleteList(parseTree);
                parseTree = NULL;
            } else {
                parseTree = parse(tokens, &depth, parseTree);
                // after parseTree, tokens is undefined

                if (depth < 0) {
                    printf("Syntax Error: too many close parentheses (start over!) \n");
                    depth = 0;
                    deleteList(parseTree);
                    parseTree = NULL;
                }
                else if (depth == 0) {
                    printConsCell(parseTree);
                    printf("\n");
                    deleteList(parseTree);
                    parseTree = NULL;
                }
            }
        }
    }
    deleteList(parseTree);
    free(expression);
    return 0;
}
*/

/* INTERPRETER MAIN */

int main(int argc, char *argv[]) {

    ConsCell *tokens = NULL, *parseTree = NULL;
    int depth = 0;
    //char *expression = malloc(256 * sizeof(char));
    char *expression = (char *)NULL;
    Environment *env = createTopFrame();

    //Load library
    int i;
    if (argc > 1 ){
         for (i = 1; i < argc; ++i) {
            char command[256];
            strcpy(command, "(load \"");
            strcat(command, argv[i]);
            strcat(command, "\")");
            int librarydepth = 0;
            ConsCell *libraryParseTree = NULL;
            Value *library = eval(parse(reverse(tokenize(command)),&librarydepth, libraryParseTree)->car, env );
            printResult(library);
        }
    }
   
    while ((expression = readline("> ")) && (*expression != 'q')) {

        tokens = tokenize(expression);
        tokens = reverse(tokens);

        // ignores when the user only inputs new lines
        if (expression[0] != 0 && expression[0] != ';') { //with readline, \n and whole comments are removed
            add_history(expression);
            if (!tokens) {
                printf("Syntax Error\n");
                deleteList(parseTree);
                parseTree = NULL;
            } else {
                parseTree = parse(tokens, &depth, parseTree);
                // after parseTree, tokens is undefined

                if (depth < 0) {
                    printf("Syntax Error: too many close parentheses (start over!) \n");
                    depth = 0;
                    deleteList(parseTree);
                    parseTree = NULL;
                }
                else if (depth == 0) {
                    // deals with the case when we enter () as input
                    if (parseTree->car->type == consType && (parseTree->car->data.cons == NULL)) {
                        printConsCell(parseTree);
                        printf("\n");
                        deleteList(parseTree);
                        parseTree = NULL;
                    }
                    else {
                        Value *result = eval(parseTree->car, env);
                        printResult(result);
                        parseTree = NULL;
                    }
                }
            }
        }      
    }

    clear_history();
    return 0;
}





