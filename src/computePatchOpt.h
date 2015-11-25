#ifndef __COMPUTEPATCHOPT_H_
#define __COMPUTEPATCHOPT_H_

#include "patchList.h"

#define MAXCOST 0xFFFFFF00

typedef struct node_s{
	int cost;
	int inLine;
	int outLine;
	patchList patch;
} node;

	void printHelp();
	void computePatch();//computes the patch :o
	int getAddCost(node *son node, *pere, char *iString, char *oString);
	int getDelCost(node *son, node *pere, char *iString, char *oString);
	int getSubstCost(node *son, node *pere, char *iString, char *oString);
	void printPatch();
	int getPatchCost();
	char *get
	

#endif
