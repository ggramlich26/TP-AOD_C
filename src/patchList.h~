#ifndef __PATCHLIST_H_
#define __PATCHLIST_H_

typedef enum{ADD, SUBST, DEL} operation;

typedef struct patchList_s{
	operation op;
	struct patchList_s *next;
	int nRef;
} patchListElement;

typedef patchListElement *patchList;

void printPatchList(patchList l);
pathList addHead(patchList l, operation op);
void incRef(patchList l);
void decRef(patchList l);

#endif
