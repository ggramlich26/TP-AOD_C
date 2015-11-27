#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "patchTable.h"
#include "stringList.h"
#include "computePatchOpt.h"

uint32_t *table;
extern int nOutput;
extern int nInput;

void initPatchTable(){
	table = malloc(sizeof(*table)*(nInput+1)*(nOutput+1));
	if(table == NULL){
		printf("Error initialising table to trace patch\n");
		exit(0);
	}
}

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

void setMultDelOp(int inLine, int outLine, int nLines){
	table[inLine*(nOutput+1)+outLine] = MULTDELOP | (nLines & LINEMASK);
}

uint32_t getOp(int inLine, int outLine){
	return table[inLine*(nOutput+1)+outLine];
}

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
