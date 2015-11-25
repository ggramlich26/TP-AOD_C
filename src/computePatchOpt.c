#include "patchList.h"
#include "computePatchOpt.h"
#include <stdio.h>
#include <string.h>

node *state;
int nInput;
int nOutput;
char *inFile;
char *outFile;

int main(int argc, char** argv){

	if(argc != 3){
		printHelp();
		return 0;
	}
}

void printHelp(){
	printf("Usage: computePatch <Input File> <Output File>\n");
}
