#include <stdlib.h>
#include <string.h>

#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastName[], entry *pHead)
{
    /* TODO: implement */
    while(pHead != NULL) {
        if(strcasecmp(lastName , pHead->lastName) == 0)
            return pHead ;
        pHead = pHead-> pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    entry* next =  malloc(sizeof(entry)) ;
    next -> pNext = NULL;
    e-> pNext = next ;
    strncpy(next->lastName , lastName , MAX_LAST_NAME_SIZE);

    return next;
}
