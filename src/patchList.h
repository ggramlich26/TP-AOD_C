#ifndef __PATCHLIST_H_
#define __PATCHLIST_H_

typedef enum{ADD, SUBST, DEL, COPY} operation;

typedef struct patchList_s{
	operation op;
	struct patchList_s *next;
	int nRef;
} patchListElement;

typedef patchListElement *patchList;

void printPatchList(patchList l);

patchList addHead(patchList l, operation op);
patchList delHead(patchList l);

void incRef(patchList l);
void decRef(patchList l);

patchList inversion(patchList l);

#endif
