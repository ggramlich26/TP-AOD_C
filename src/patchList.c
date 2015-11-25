#include "patchList.h"
#include <stdio.h>
#include <stdlib.h>

void printPatchList(patchList l) {
    l = inversion(l);
    char string[12];
    int i = 0;
    
    while (l != NULL) {
        i++;
        printf("%d ", i);
        
        switch (l->op) {
            case ADD:
                printf("+");
                break;
            case DEL:
                printf("-");
                break;
            case SUBST:

                break;
                
            default:
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