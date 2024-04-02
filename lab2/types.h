#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned int hash_t;
typedef struct node {
	hash_t k;
	void* data;
	struct node* next;
	struct node* prev;
	
} node;

typedef struct hashBucket {
	node* beginningOfList;
	struct hashBucket* bucketNext;
	struct hashBucket* bucketPrev;
} hashBucket;

hash_t f_hash(void* nd);

int compare_hash(hash_t, hash_t);

void print(node*);

typedef struct HashMap {
	hashBucket* blocks;
	hash_t(*f_hash)(void*);
	int (*comparator)(hash_t, hash_t); 
	void (*print)(node*);
} HashMap;

#endif