#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringList.h"

stringList addStringHead(stringList l, char *str){
	stringList tmp = malloc(sizeof(*tmp));
	tmp->str = malloc(sizeof(*(tmp->str))*(strlen(str)+1));
	strcpy(tmp->str, str);
	tmp->next = l;
	return tmp;
}

void deleteStringList(stringList l){
	while(l != NULL){
		stringList tmp = l->next;
		free(l->str);
		free(l);
		l = tmp;
	}
}

void printStringList(stringList l){
	while(l != NULL){
		printf("%s", l->str);
		l = l->next;
	}
}
