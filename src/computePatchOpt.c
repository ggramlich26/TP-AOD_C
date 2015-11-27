/**
*@mainpage TP AOD en temps libre
* 
* This program is ment to calculate an optimal patch in order to transform one file into another.
*
*/

/*! \file computePatchOpt.c
 *  \brief	   This is used to calculate the optimal patch
 *  \author    Georg Gramlich
 *  \version   1.0
 *  \date      2015
 */

#include "patchTable.h"
#include "computePatchOpt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

/**
* state is used to memorize the last state. It is needed to calculate the next state. A state is the optimal patch and especially its cost for theith input line and all output lines.
* multDel is used to memorize the best reachable cost with a multiple deletion. This is memorized for all line numbers of the output file.
*/
node **state;
node **multDel;

uint8_t printEnabled = 1;

int nInput;
int nOutput;
char *inFile;
char *outFile;
char **outFileBuffer;
FILE *inFilep;
FILE *outFilep;

int main(int argc, char** argv){

	if(argc != 3){
		printHelp();
		return 0;
	}
	inFile = argv[1];
	outFile = argv[2];
	clock_t t1 = clock();

	init();
	computePatch();


	if(printEnabled)
		printPatch();
	clock_t t2 = clock();

	//printf("Calculating took: %fms\n", (((float)t2 - (float)t1) / 1000000.0F ) * 1000);

	cleanup();
}

void printHelp(){
	printf("Usage: computePatch <Input File> <Output File>\n");
}

/**
* @brief Function doing all the initialisation
*
* In this function the line numbers of the input and output file are determined and all structures, such as state multDel and the patchTable are initialized.
*/
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
	nOutput = lines;
	rewind(f);
	outFileBuffer = malloc(sizeof(*outFileBuffer) * nOutput);
	char str[MAXSTRINGSIZE];
	for(int i = 0; i < nOutput; i++){
		fgets(str, MAXSTRINGSIZE, f);
		outFileBuffer[i] = malloc((strlen(str)+1)*sizeof(*str));
		strcpy(outFileBuffer[i], str);
	}
	fclose(f);
	
	//init patchTable
	if(printEnabled)
		initPatchTable();

	//init Filepointers
	inFilep = fopen(inFile, "rt");
	outFilep = fopen(outFile, "rt");

	//init state array
	state = calloc(nOutput + 1, sizeof(*state));
	state[0] = calloc(1, sizeof(**state));
	state[0]->cost = 0;
	state[0]->inLine = 0;
	state[0]->outLine = 0;
	//state[0]->patch = NULL;
	for(int i = 1; i < nOutput + 1; i++){
		char *str = getOutLine(i);
		node *n = calloc(1, sizeof(*n));
		n->cost = getAddCost(state[i], state[i-1], str);
		//n->patch = addHead(state[i-1]->patch, ADD);
		setOp(0, i, ADD);
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

/**
* @brief This function returns the specified line of the input file.
*
* @param str This is a char pointer to a string, in wich the line will be stored. The user has to make sure the memory space is large enough.
* @param line specifies which line to read
*/
void getInLine(char *str, int line){
	static int myLine = 1;
	if(line < myLine){
		rewind(inFilep);
		myLine = 1;
	}
	while(myLine <= line){
		myLine++;
		fgets(str, MAXSTRINGSIZE, inFilep);
	}
}

/**
* @brief returns the specified line of the output file
*
* This function returns a pointer to a string containing the specified line of the output file. The whole output file is loaded in the init() function, so this function works very rapidly.
*
* @param line specifies which line to read
*
* @return pointer to the string containing the specified line
*
*/
char* getOutLine(int line){
	return outFileBuffer[line-1];
}

/**
* @brief Function freeing all the memory allocated before
*
* This function has to be called exactly once at the end of the calculation. After calling this function there will be no more calculations and prints possible.
*/
void cleanup(){
	if(inFilep != NULL){
		fclose(inFilep);
	}
	if(outFilep != NULL){
		fclose(outFilep);
	}
	//cleanup multDel
	for(int i = 0; i < nOutput + 1; i++){
		free(multDel[i]);
	}
	free(multDel);
	//free state
	for(int i = 0; i < nOutput+1; i++){
		free(state[i]);
		free(outFileBuffer[i]);
	}
	free(state);
	free(outFileBuffer);
}


/**
* @brief calculates the cost for an addition for the specified node
*
* @return the cost
*/
int getAddCost(node *son, node *pere, char *oString){
	if(oString[strlen(oString)-1] == '\n')
		return pere->cost + 10 + strlen(oString) -1;
	else
		return pere->cost + 10 + strlen(oString);
}

/**
* @brief calculates the cost for an addition for the specified node
*
* @return the cost
*/
int getDelCost(node *son, node *pere){
	return pere->cost + 10;
}

/**
* @brief calculates the cost for a Deletion for the specified node
*
* @return the cost
*/
int getMultDelCost(node *son){
	return multDel[son->outLine]->cost + 15;
}

/**
* @brief calculates the cost for a multiple deletion for the specified node
*
* @return the cost
*/
int getMultDelLines(node *son){
	return son->inLine - multDel[son->outLine]->inLine;
}

/**
* @brief calculates the cost for a substitution for the specified node. A substitution can also be a copy!
*
* @return the cost
*/
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

/**
* Updates the multDel array with the possibly new best reachable cost
*/
void updateDel(node *n){
	if(n->cost < multDel[n->outLine]->cost){
		multDel[n->outLine]->cost = n->cost;
		multDel[n->outLine]->inLine = n->inLine;
		multDel[n->outLine]->outLine = n->outLine;
	}
}

/**
* @Brief computes the actual patch
*
* This function is used to compute the actual patch based on an iterative algorithm.
*/
void computePatch(){
	//printStateCost();
	node *old = calloc(1, sizeof(*old));
	node *new = calloc(1, sizeof(*new));
	char iString[MAXSTRINGSIZE];
	for(int i = 1; i <= nInput; i++){
		getInLine(iString, i);
		for(int j = 0; j < nOutput+1; j++){
			treatNode(j, new, old, iString);
			if(j == nOutput){
				node *tmp = state[j];
				node *tmp2 = state[j-1];
				state[j] = new;
				state[j-1] = old;
				new = tmp;
				old = tmp2;
			}
			else if(j != 0){
				node *tmp = state[j-1];
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
	}
}

/**
* @brief updates a cell in the state array
*
* This function will calculate the best cost for a not treated line number combination. It will also make sure the multDel array and the patchTable are updated.
*
* @param index the index of the state cell to be treated
* @param me where to save the results
* @param addNode node which can be reached by an addition
* @param iString string of the corresponding line of the input file
*/
void treatNode(int index, node *me, node *addNode, char *iString){
	me->inLine = state[index]->inLine + 1;
	me->outLine = state[index]->outLine;
	if(me->outLine == 0){
		me->cost = getDelCost(me, state[index]);
		updateDel(me);
		return;
	}
	char *oString = getOutLine(me->outLine);
	if(me->inLine == 0){
		me->cost = getAddCost(me, addNode, oString);
		updateDel(me);
		return;
	}

	int addCost = getAddCost(me, addNode, oString);
	int delCost = getDelCost(me, state[index]);
	int multDelCost = getMultDelCost(me);
	int substCost = getSubstCost(me, state[index-1], iString, oString);
	if(substCost <= multDelCost && substCost < delCost && substCost <= addCost){
		me->cost = substCost;
		if(substCost == state[index-1]->cost){
			if(printEnabled)
				setOp(me->inLine, me->outLine, COPY);
		}
		else{
			if(printEnabled)
				setOp(me->inLine, me->outLine, SUBST);
		}
		updateDel(me);
	}
	else if(multDelCost < delCost && multDelCost < addCost){
		me->cost = multDelCost;
		if(printEnabled)
			setMultDelOp(me->inLine, me->outLine, getMultDelLines(me));
		updateDel(me);
	}
	else if(addCost < delCost){
		me->cost = addCost;
		if(printEnabled)
			setOp(me->inLine, me->outLine, ADD);
		updateDel(me);
	}
	else{
		me->cost = delCost;
		if(printEnabled)
			setOp(me->inLine, me->outLine, DEL);
		updateDel(me);
	}
}

void printPatch(){
	printPatchTable(nInput, nOutput);
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
