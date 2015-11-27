/*! \file patchTable.c
 *  \brief	   This implements a table with the optimal operation chose for each combination of input and output lines. This is necessary to trace back the path to the optimal solution and to create the actual patch.
 *  \author    Georg Gramlich
 *  \author	   Robin Fleury-Elrharby
 *  \version   1.0
 *  \date      2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "patchTable.h"
#include "stringList.h"
#include "computePatchOpt.h"

/**
* table is a table of size ((nInput+1)*(nOutput+1)) which stores for each combination of input and output lines wich choise (addition, copy, substitution, deletion, multiple deletion) has led to its optimal cost. It can therefor be used to track back the path taken to reach the end and to create the patch output
*/
uint32_t *table;
extern int nOutput;
extern int nInput;

/**
* @brief does all the initialisations necessary for the patchTable
*/
void initPatchTable(){
	table = malloc(sizeof(*table)*(nInput+1)*(nOutput+1));
	if(table == NULL){
		printf("Error initialising table to trace patch\n");
		exit(0);
	}
}

/**
* @brief sets the specified field in the patchTable to the specified operation
*
* @param inLine line of input File
* @param outLine line of output File
* @param op operation, that has led to the minimal cost for the combination (inLine, outLine)
*/
void setOp(int inLine, int outLine, operation op){
	if(op == ADD){
		table[inLine*(nOutput+1)+outLine] = ADDOP;
	}
	else if(op == SUBST){
		table[inLine*(nOutput+1)+outLine] = SUBSTOP;
	}
	else if(op == COPY){
		table[inLine*(nOutput+1)+outLine] = COPYOP;
	}
	else if(op == DEL){
		table[inLine*(nOutput+1)+outLine] = DELOP;
	}
}

/**
* same as setOp, but adapted to multiple deletion, as this operation requires to know how many lines should be deleted
*/
void setMultDelOp(int inLine, int outLine, int nLines){
	table[inLine*(nOutput+1)+outLine] = MULTDELOP | (nLines & LINEMASK);
}

uint32_t getOp(int inLine, int outLine){
	return table[inLine*(nOutput+1)+outLine];
}

/**
* this operation creates a stringList containing the patch and prints it
*/
void printPatchTable(int inLine, int outLine){
	stringList l = NULL;
	while(inLine > 0 || outLine > 0){
		uint32_t op = getOp(inLine, outLine);
		if(op == ADDOP){
			l = addStringHead(l, getOutLine(outLine));
			char str[16];
			sprintf(str, "+ %d\n", inLine);
			l = addStringHead(l, str);
			outLine--;
		}
		else if(op == SUBSTOP){
			l = addStringHead(l, getOutLine(outLine));
			char str[16];
			sprintf(str, "= %d\n", inLine);
			l = addStringHead(l, str);
			outLine--;
			inLine--;
		}
		else if(op == COPYOP){
			outLine--;
			inLine--;
		}
		else if(op == DELOP){
			char str[16];
			sprintf(str, "d %d\n", inLine);
			l = addStringHead(l, str);
			inLine--;
		}
		else if((op & 0xF0000000) == MULTDELOP){
			uint32_t nLines = op & LINEMASK;
			char str[16];
			sprintf(str, "D %d %d\n", inLine-nLines+1, nLines);
			l = addStringHead(l, str);
			inLine -= nLines;
		}
	}
	printStringList(l);
}
