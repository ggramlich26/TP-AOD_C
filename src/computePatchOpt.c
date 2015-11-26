#include "patchList.h"
#include "computePatchOpt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

node **state;
node **multDel;

int nInput;
int nOutput;
char *inFile;
char *outFile;
FILE *inFilep;
FILE *outFilep;

int main(int argc, char** argv){

	if(argc != 3){
		printHelp();
		return 0;
	}
	inFile = argv[1];
	outFile = argv[2];
	printf("%s   %s\n", inFile, outFile);
	init();
	computePatch();
	printPatch();
	cleanup();
}

void printHelp(){
	printf("Usage: computePatch <Input File> <Output File>\n");
}

void init(){
	//getting number of lines in input and output file
	FILE *f = fopen(inFile, "rt");
	if(f == NULL){
		printf("Could not open File %s. Exiting\n", inFile);
		exit(0);
	}
	int lines = 0;
	char ch;
	while(!feof(f)){
		ch = fgetc(f);
		if(ch == '\n'){
			lines++;
		}
	}
	fclose(f);
	nInput = lines;
	f = fopen(outFile, "rt");
	if(f == NULL){
		printf("Could not open File %s. Exiting\n", outFile);
		exit(0);
	}
	lines = 0;
	while(!feof(f)){
		ch = fgetc(f);
		if(ch == '\n'){
			lines++;
		}
	}
	fclose(f);
	nOutput = lines;

	//init Filepointers
	inFilep = fopen(inFile, "rt");
	outFilep = fopen(outFile, "rt");

	//init state array
	state = calloc(nOutput + 1, sizeof(*state));
	state[0] = calloc(1, sizeof(**state));
	state[0]->cost = 0;
	state[0]->inLine = 0;
	state[0]->outLine = 0;
	state[0]->patch = NULL;
	for(int i = 1; i < nOutput + 1; i++){
		char str[MAXSTRINGSIZE];
		getOutLine(str, i);
		node *n = calloc(1, sizeof(*n));
		n->cost = getAddCost(state[i], state[i-1], str);
		n->patch = addHead(state[i-1]->patch, ADD);
		n->inLine = 0;
		n->outLine = i;
		state[i] = n;
	}

	//init multDel
	multDel= calloc((nOutput+1), sizeof(*multDel));
	for(int i = 0; i < nOutput+1; i++){
		multDel[i] = calloc(1, sizeof(**multDel));
		multDel[i]->cost = MAXCOST;
	}

}

void getInLine(char *str, int line){
	static int myLine = 1;
	if(line < myLine){
		if(inFilep != NULL){
			fclose(inFilep);
		}
		inFilep = fopen(inFile, "r");
		myLine = 1;
	}
	while(myLine <= line){
		myLine++;
		fgets(str, MAXSTRINGSIZE, inFilep);
	}
}

void getOutLine(char *str, int line){
	static int myLine = 1;
	if(line < myLine){
		if(outFilep != NULL){
			fclose(outFilep);
		}
		outFilep = fopen(outFile, "rt");
		if(NULL == outFilep){
			printf("Could not open file %s\n", outFile);
			exit(0);
		}
		myLine = 1;
	}
	while(myLine <= line){
		myLine++;
		fgets(str, MAXSTRINGSIZE, outFilep);
	}
}

void cleanup(){
	if(inFilep != NULL){
		fclose(inFilep);
	}
	if(outFilep != NULL){
		fclose(outFilep);
	}
	//cleanup multDel
	for(int i = 0; i < nOutput + 1; i++){
		if(multDel[i]->patch != NULL){
			decRef(multDel[i]->patch);
		}
		free(multDel[i]);
	}
	free(multDel);
	//free state
	for(int i = 0; i < nOutput+1; i++){
		decRef(state[i]->patch);
		free(state[i]);
	}
	free(state);
}


int getAddCost(node *son, node *pere, char *oString){
	if(oString[strlen(oString)-1] == '\n')
		return pere->cost + 10 + strlen(oString) -1;
	else
		return pere->cost + 10 + strlen(oString);
}

patchList getAddPatch(node *son, node *pere, char *oString){
	return addHead(pere->patch, ADD);
}

int getDelCost(node *son, node *pere){
	if(pere->cost + 10 < multDel[son->outLine]->cost + 15)
		return pere->cost + 10;
	else
		return multDel[son->outLine]->cost + 15;
}

patchList getDelPatch(node *son, node *pere){
	if(pere->cost + 10 < multDel[son->outLine]->cost + 15)
		return addHead(pere->patch, DEL);
	else{
		patchList l = multDel[son->outLine]->patch;
		for(int i = 0; i < son->inLine - multDel[son->outLine]->inLine; i++){
			l = addHead(l, DEL);
		}
		return l;
	}
}

int getSubstCost(node *son, node *pere, char *iString, char *oString){
	if(strcmp(iString, oString) == 0){
		return pere->cost;
	}
	else{
		if(oString[strlen(oString)-1] == '\n')
			return pere->cost + 10 + strlen(oString) -1;
		else
			return pere->cost + 10 + strlen(oString);
	}
}

patchList getSubstPatch(node *son, node *pere, char *iString, char *oString){
	if(strcmp(iString, oString) == 0){
		return addHead(pere->patch, COPY);
	}
	else{
		return addHead(pere->patch, SUBST);
	}
}

void updateDel(node *n){
	if(n->cost < multDel[n->outLine]->cost){
		decRef(multDel[n->outLine]->patch);
		incRef(n->patch);
		multDel[n->outLine]->cost = n->cost;
		multDel[n->outLine]->inLine = n->inLine;
		multDel[n->outLine]->patch = n->patch;
		multDel[n->outLine]->outLine = n->outLine;
	}
}

void computePatch(){
	//printStateCost();
	node *old = calloc(1, sizeof(*old));
	node *new = calloc(1, sizeof(*new));
	for(int i = 1; i <= nInput; i++){
		printf("%d\n", (int)(i*100.0 / nInput));
		for(int j = 0; j < nOutput+1; j++){
			treatNode(j, new, old);
			if(j == nOutput){
				node *tmp = state[j];
				node *tmp2 = state[j-1];
				decRef(tmp->patch);
				state[j] = new;
				state[j-1] = old;
				new = tmp;
				old = tmp2;
			}
			else if(j != 0){
				node *tmp = state[j-1];
				decRef(tmp->patch);
				state[j-1] = old;
				old = new;
				new = tmp;
			}
			else if(j == 0){
				node *tmp = new;
				new = old;
				old = tmp;
			}
		}
		//printStateCost();
	}
}

void treatNode(int index, node *me, node *addNode){
	me->inLine = state[index]->inLine + 1;
	me->outLine = state[index]->outLine;
	char iString[MAXSTRINGSIZE];
	char oString[MAXSTRINGSIZE];
	if(me->outLine == 0){
		me->cost = getDelCost(me, state[index]);
		me->patch = getDelPatch(me, state[index]);
		updateDel(me);
		return;
	}
	getOutLine(oString, me->outLine);
	if(me->inLine == 0){
		me->cost = getAddCost(me, addNode, oString);
		me->patch = getAddPatch(me, addNode, oString);
		updateDel(me);
		return;
	}
	getInLine(iString, me->inLine);
	if(index == nOutput-1){
		;
	}

	int addCost = getAddCost(me, addNode, oString);
	int delCost = getDelCost(me, state[index]);
	int substCost = getSubstCost(me, state[index-1], iString, oString);
	//printf("%d %d %d\n", addCost, delCost, substCost);
	if(substCost <= delCost && substCost <= addCost){
		me->cost = substCost;
		me->patch = getSubstPatch(me, state[index-1], iString, oString);
		updateDel(me);
	}
	else if(addCost < delCost){
		me->cost = addCost;
		me->patch = getAddPatch(me, addNode, oString);
		updateDel(me);
	}
	else{
		me->cost = delCost;
		me->patch = getDelPatch(me, state[index]);
		updateDel(me);
	}
}

void printPatch(){
	//printPatchListBackward(state[nOutput]->patch);
	printPatchList(state[nOutput]->patch);
}

void printStateCost(){
	printf("s: ");
	for(int i = 0; i < nOutput+1; i++){
		printf("%2d ", state[i]->cost);
	}
	printf("   d: ");
	for(int i = 0; i < nOutput+1; i++){
		printf("%2d ", multDel[i]->cost);
	}
	printf("\n");
}
