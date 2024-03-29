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
	void init();
	void cleanup();
	void computePatch();//computes the patch :o
	void treatNode(int index, node *me, node *addNode);
	int getAddCost(node *son, node *pere, char *oString);
	int getDelCost(node *son, node *pere);
	int getSubstCost(node *son, node *pere, char *iString, char *oString);
	void printPatch();
	int getPatchCost();
	void getInLine(char *str, int line);
	void getOutLine(char *str, int line);
	

#endif
