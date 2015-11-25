#include "patchList.h"
#include "computePatchOpt.h"
#include <stdio.h>
#include <stdlib.h>

void printPatchList(patchList l) {
    l = inversion(l);
    char string[1024];
    int iIn = 0, iOut = 0;
    
//    “+ k\n” Ajout : la ligne suivante dans le patch est ajoutée sur le flot de sortie après la ligne k du flot d’entrée. Si k = 0, la ligne est insérée avant la première ligne (numérotée 1) du flot d’entrée.
//    “= k\n” Substitution : la ligne k du flot d’entrée est remplacée sur le flot de sortie par la ligne suivante dans le patch.
//    “d k\n” Destruction : la ligne k du flot d’entrée n’est pas recopiée sur le flot de sortie.
    
    while (l != NULL) {
		//printf("iIn: %d, iOut: %d\n", iIn, iOut);
        
        switch (l->op) {
            case ADD:
                printf("+ %d\n", iIn);
                
                iOut++;
                
                getOutLine(string, iOut);
                printf("%s\n", string);
                break;
                
            case DEL:
                printf("- %d\n", iIn+1);
                
                iIn++;
                break;
                
            case SUBST:
                printf("= %d\n", iIn+1);
                
                iIn++;
                iOut++;
                getOutLine(string, iOut);
                printf("%s\n", string);
                break;
                
            case COPY:
                iIn++;
                iOut++;
                break;
        }
		l = l->next;
    }
}

void printPatchListWithLineNumbers(patchList l) {
    l = inversion(l);
    char string[512];
    int i = 0, iIn = 0, iOut = 0;
    
//    “+ k\n” Ajout : la ligne suivante dans le patch est ajoutée sur le flot de sortie après la ligne k du flot d’entrée. Si k = 0, la ligne est insérée avant la première ligne (numérotée 1) du flot d’entrée.
//    “= k\n” Substitution : la ligne k du flot d’entrée est remplacée sur le flot de sortie par la ligne suivante dans le patch.
//    “d k\n” Destruction : la ligne k du flot d’entrée n’est pas recopiée sur le flot de sortie.
    
    while (l != NULL) {
		//printf("iIn: %d, iOut: %d\n", iIn, iOut);
        
        switch (l->op) {
            case ADD:
                i++;
                printf("%d + %d\n",i, iIn);
                
                iOut++;
                i++;
                
                getOutLine(string, iOut);
                printf("%d %s", i, string);
                break;
                
            case DEL:
                i++;
                printf("%d - %d\n",i, iIn+1);
                
                iIn++;
                break;
                
            case SUBST:
                i++;
                printf("%d = %d\n", i, iIn+1);
                
                iIn++;
                iOut++;
                i++;
                getOutLine(string, iOut);
                printf("%d %s", i, string);
                break;
                
            case COPY:
                iIn++;
                iOut++;
                break;
        }
		l = l->next;
    }
}

patchList addHead(patchList l, operation op) {
    // creation
    patchListElement* new = malloc(sizeof(patchListElement));
    new->op = op;
    new->next = l;
    new->nRef = 1;
    
    // actualisation
    if(l!=NULL)
        incRef(l);
    
    return new;
}

patchList delHead(patchList l) {
    patchList old = l->next;
    
    if (old!=NULL)
        decRef(old);
    
    free(l);
    
    return old;
    
}

void incRef(patchList l) {
    l->nRef++;
}

void decRef(patchList l) {
	if(NULL == l){
		return;
	}

    l->nRef--;
    
    if (l->nRef == 0) {
        delHead(l);
    }
}

patchList inversion(patchList l) {
    
    if (l == NULL) {
        return l;
    }
    
    patchList pointeur = l, precPointeur = NULL, nextPointeur = pointeur->next;
    
    while (nextPointeur != NULL) {
        // <-P A-> N-> devient <-P <-A N->
        pointeur->next =  precPointeur;
        
        // <-P <-A N-> devient <-A N-> N'->

        precPointeur = pointeur;
        
        pointeur = nextPointeur;
        
        nextPointeur = nextPointeur->next;
    }
    
    // <-P A-> X devient <-P <-A
    pointeur->next = precPointeur;
    
    return pointeur;
}
