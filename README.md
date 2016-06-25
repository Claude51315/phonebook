Licensing
---------
`phonebook` is freely redistributable under the two-clause BSD License.
Use of this source code is governed by a BSD-style license that can be found
in the `LICENSE` file.


# Development log
[成大資工2016嵌入式系統作業一：電話簿](http://wiki.csie.ncku.edu.tw/embedded/2016q1h1)

[HACKMD連結](https://hackmd.io/s/H1VTlDqS)
## environment 
- ubuntu 15.10
- ```lscpu```![cpuinfo](https://i.imgur.com/3Fl8wPE.png)

## Improve1: lower cache miss
Only keep the index value of phone book entry
- Original struct:
```clike=
typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
    struct __PHONE_BOOK_ENTRY *pNext;
} entry;

```
size = 136 byte
- modified struct:
```clike=
typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    entry *pNext;
    detail* pDetail;
} entry;
```
size = 32 byte
- cache miss comparison 
	
	```
	perf stat -- repeat 100 \
	-e cache-misses, cache-references, instructions, cycles \ 
	./[program executable name]
	```
	- original struct ![](https://i.imgur.com/bDRI3zG.png)
	- modified struct ![](https://i.imgur.com/SuBV3W6.png)


![](https://i.imgur.com/ZzEKPwK.png)

# 觀察程式熱點：
``` $
	$ perf record -e cycles./phonebook_orig
	$ perf report
```
![](https://i.imgur.com/8s7X2O9.png)
可以發現 findName()和malloc() 花的時間較多
## Improve2 : using hash function
利用hash來減少findName的執行時間。
- basic hash function: hashIndex = sum(lastname)% tableSize
```clike=
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

```
## Improve3 : memory pool
實作memory pool減少每次建立新entry時都呼叫malloc()的次數。
POOL_SIZE 預設值為40000，太小的話則會因為不停的新增pool而增加執行時間。
```clike=
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

```
# 綜合比較
![](https://i.imgur.com/tkFBy2Z.png)
- 可以看到hash大大的減少了findName的時間，而hash版本的append()只需要增加hash table中的bucket,不用從頭開始找，append()的執行時間也減少了。
- 使用memory pool 的話，則更進一步的減少append()的時間。
- hash + memory pool 一起使用的話，append()又增加了些許的時間，推測原因來自於hash table 和 memory pool操作所需的時間。
- cache miss 比較
	- original ![](https://i.imgur.com/rcb35Lt.png)
	- hash ![](https://i.imgur.com/Urjrs6X.png)
	- memory pool ![](https://i.imgur.com/jvXfaw4.png)
	- hash + memory pool ![](https://i.imgur.com/ZND0fdT.png)





