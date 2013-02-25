#Makefile for project
FLAGS=-g
DEPS = header.h

interpreter: main.c tokenizer.c linkedList.c bindingList.c environmentList.c parser.c interpreter.c primitiveFunction.c $(DEPS)
	gcc $^ -lreadline -o $@

debug: main.c tokenizer.c linkedList.c bindingList.c environmentList.c parser.c interpreter.c primitiveFunction.c  $(DEPS)
	gcc $(FLAGS) $^ -lreadline -o $@

