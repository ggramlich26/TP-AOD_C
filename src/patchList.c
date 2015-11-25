#include "patchList.h"
#include <stdio.h>
#include <stdlib.h>

void printPatchList(patchList l) {
    l = inversion(l);
    char string[12];
    int i = 0, iIn = 0, iOut = 0;
    
//    “+ k\n” Ajout : la ligne suivante dans le patch est ajoutée sur le flot de sortie après la ligne k du flot d’entrée. Si k = 0, la ligne est insérée avant la première ligne (numérotée 1) du flot d’entrée.
//    “= k\n” Substitution : la ligne k du flot d’entrée est remplacée sur le flot de sortie par la ligne suivante dans le patch.
//    “d k\n” Destruction : la ligne k du flot d’entrée n’est pas recopiée sur le flot de sortie.
    
    while (l != NULL) {
        
        switch (l->op) {
            case ADD:
                i++;
                printf("%d + %d\n",i, iOut);
                
                iOut++;
                i++;
                
                getOutLine(string, iOut);
                printf("%d %s", i, string);
                break;
                
            case DEL:
                i++;
                printf("%d - %d\n",i, iOut+1);
                
                iIn++;
                break;
                
            case SUBST:
                i++;
                printf("%d = %d", i, iOut+1);
                
                iIn++;
                iOut++;
                i++;
                getOutLine(string, iOut+1);
                printf("%d %s", i, string);
                break;
                
            case COPY:
                iIn++;
                iOut++;
                break;
        }
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