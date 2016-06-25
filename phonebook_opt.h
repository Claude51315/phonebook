#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H
#include<stdio.h>

#define MAX_LAST_NAME_SIZE 16

/* TODO: After modifying the original version, uncomment the following
 * line to set OPT properly */
#define OPT 1
#define POOL_SIZE 40000
/*
 * forward declaration
 */
typedef struct __PHONE_BOOK_ENTRY entry;
typedef struct __PHONE_BOOK_DETAIL detail;
typedef struct __PHONE_BOOK_HASH_TABLE hashTable ;
typedef unsigned int hashIndex ;
typedef struct __PHONE_BOOK_MEMORY_POOL pool ;


/*
 * basic struct
 */
typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    entry *pNext;
    detail* pDetail;
} entry;
typedef struct __PHONE_BOOK_DETAIL {
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
} detail;

typedef struct __PHONE_BOOK_HASH_TABLE {
    unsigned int  tableSize ;
    entry **list ;
} hashTable;

typedef struct __PHONE_BOOK_MEMORY_POOL {
    entry* entryPool;
    entry* cur;
    unsigned int count ;
    pool* pNext;
} pool ;

/*
 * original
 */
entry *findName(char lastname[], entry *pHead);
entry *append(char lastName[], entry *e);

/*
 * memory pool
 */
pool* createNewPool() ;
entry* allocateFromPool(pool* pHead);
/*
 * hash function
 */
hashTable* createHashTable(int tableSize);
hashTable* hashAppend(char lastName[] ,hashTable* ht , pool* p );
entry* hashFindName(char lastName[] , hashTable* ht);
hashIndex hash1(char* key , hashTable *ht) ;

#endif

