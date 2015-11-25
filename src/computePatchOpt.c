#include "patchList.h"
#include "computePatchOpt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

node **state;
int *multDelCost;
int *multDelLine;

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
	init();
}

void printHelp(){
	printf("Usage: computePatch <Input File> <Output File>\n");
}

void init(){
	FILE *f = fopen(inFile, "r");
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
	f = fopen(outFile, "r");
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

	state = calloc(nOutput + 1, sizeof(*state));
	state[0] = calloc(1, sizeof(**state));
	state[0]->cost = 0;
	state[0]->inLine = 0;
	state[0]->outLine = 0;
	state[0]->patch = NULL;
	for(int i = 1; i < nOutput + 1; i++){
		char str[512];
		getOutLine(i, str);
		node *n = calloc(1, sizeof(*n));
		n->cost = getAddCost(state[i], state[i-1], str);
		n->patch = addHead(state[i-1]->patch, ADD);
		n->inLine = 0;
		n->outLine = i;
		state[i] = n;
	}

	//init multDel
	multDelCost = calloc((nOutput+1), sizeof(*multDelCost));
	multDelLine = calloc((nOutput+1), sizeof(*multDelLine));
	for(int i = 0; i < nOutput+1; i++){
		multDelCost[i] = MAXCOST;
	}

	//init Filepointers
	inFilep = fopen(inFile, "r");
	outFilep = fopen(outFile, "r");
}

void *getInLine(char *str, int line){
	static int myLine = 1;
	if(line < myLine){
		if(inFilep != NULL){
			flose(inFilep);
		}
		inFilep = fopen(inFile, "r");
	}
	while(myLine <= line){
		myLine++;
		fgets(str, sizeof(str), inFilep);
	}
}

void *getOutLine(char *str, int line){
	static int myLine = 1;
	if(line < myLine){
		if(outFilep != NULL){
			flose(outFilep);
		}
		outFilep = fopen(outFile, "r");
	}
	while(myLine <= line){
		myLine++;
		fgets(str, sizeof(str), outFilep);
	}
}