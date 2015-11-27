#ifndef __STRINGLIST_H_
#define __STRINGLIST_H_

typedef struct stringList_h{
	char *str;
	struct stringList_h *next;
} *stringList;

stringList addStringHead(stringList l, char *str);
void deleteStringList(stringList l);
void printStringList(stringList l);

#endif
