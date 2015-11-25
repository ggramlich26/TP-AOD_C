#include "patchList.h"
#include "computePatchOpt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

node **state;

int nInput;
int nOutput;
char *inFile;
char *outFile;

int main(int argc, char** argv){

	if(argc != 3){
		printHelp();
		return 0;
	}
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
	f = flopen(outFile, "r");
	if(f == NULL){
		printf("Could not open File %s. Exiting\n", outFile);
		exit(0);
	}

	state = calloc(nOutput + 1, sizeof(*state));
	state[0] = calloc(1, sizeof(**state));
	state[0]->cost = 0;
	state[0]->inLine = 0;
	state[0]->outLine = 0;
	state[0]->patch = NULL;
	for(int i = 1; i < nOutput + 1; i++){
		char *str = getOutLine(i);
		node *n = calloc(1, sizeof(*n));
		n->cost = getAddCost(state[i], state[i-1], str);
		n->patch = addHead(state[i-1]->patch, ADD);
		n->inLine = 0;
		n->outLine = i;
		state[i] = n;
	}
}
