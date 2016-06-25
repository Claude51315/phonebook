#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
/* original version od findName and append*/
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
/* implementation of hash*/
hashTable* createHashTable(int tableSize)
{
    hashTable* ht = NULL;
    ht = malloc(sizeof(hashTable)) ;
    if(ht == NULL) {
        assert("no enough memory to create hashTable struct");
        return NULL;
    }
    ht->tableSize = tableSize;
    ht->list = malloc(sizeof(entry*) * tableSize);
    if(ht->list == NULL) {
        assert("no enough memory to create hashTable entry list");
    }
    int i ;
    for(i = 0 ; i < tableSize ; i++ ) {
        ht->list[i] = NULL ;
    }
    return ht ;
}
hashTable* hashAppend(char lastName[] , hashTable* ht )
{
    hashIndex hi = hash1(lastName , ht);
    entry* newEntry = malloc(sizeof(entry)) ;
    if(newEntry == NULL) {
        assert("no enough memory for entry!");
        return NULL;
    }
    strcpy(newEntry->lastName , lastName);
    newEntry->pNext = ht->list[hi] ;
    ht->list[hi] = newEntry ;
    return ht;
}
entry* hashFindName(char lastName[] , hashTable* ht)
{
    hashIndex hi = hash1(lastName , ht);
    entry* i  ;
    for(i = ht->list[hi] ; i != NULL ; i = i->pNext) {
        if(strcasecmp(i->lastName , lastName) == 0)
            return i ;
    }
    return NULL ;
}
hashIndex hash1 (char* key , hashTable *ht)
{
    unsigned int sum = 0 ;
    while(*key != '\0') {
        sum = sum + *key++ ;
    }
    return sum % ht->tableSize;
}
/* memory pool */
pool* createNewPool()
{
    pool* newPool = malloc(sizeof(pool)) ;
    if(newPool == NULL) {
        assert("no enough memory for newPool struct");
        return NULL;
    }
    newPool->entryPool = malloc(sizeof(entry) * POOL_SIZE);
    if(newPool->entryPool == NULL) {
        assert("no enough memory for newPool allocation");
        return NULL ;
    }
    newPool->count = 0 ;
    newPool->pNext = NULL;
    newPool->cur = newPool->entryPool ;
    return newPool ;
}
entry* allocateFromPool(pool* pHead)
{
    entry* p;
    if(pHead->count < POOL_SIZE ) {
        p = pHead->cur ;
        pHead->cur ++ ;
        pHead->count ++ ;
        //printf("pHead->count = %d\n" , pHead->count  );
        return p ;
    } else {

        if(pHead->pNext ==NULL) {
            pHead->pNext = createNewPool();
        }
        return allocateFromPool(pHead->pNext);
    }
}
entry *poolAppend(char lastName[], entry *e , pool* p )
{
    entry* newEntry =  allocateFromPool(p) ;
    newEntry -> pNext = NULL;
    e-> pNext = newEntry ;
    strncpy(newEntry->lastName , lastName , MAX_LAST_NAME_SIZE);
    return newEntry;
}
/* hash + pool Append */
hashTable* hashPoolAppend(char lastName[] , hashTable* ht , pool* p)
{
    hashIndex hi = hash1(lastName , ht);
    entry* newEntry = allocateFromPool(p) ;
    if(newEntry == NULL) {
        assert("no enough memory for entry!");
        return NULL;
    }
    strcpy(newEntry->lastName , lastName);
    newEntry->pNext = ht->list[hi] ;
    ht->list[hi] = newEntry ;
    return ht;
}



