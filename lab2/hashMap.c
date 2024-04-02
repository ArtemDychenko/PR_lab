#include "hashMap.h"

typedef unsigned int hash_t;
hash_t f_hash(void* f) {
	return 43;
}

int compare_hash(hash_t t1, hash_t t2) {
	return 1;
}

void print(node* p) {
}

HashMap* init_hash_map(hash_t(*f_hash)(void*), int(*compare_hash)(hash_t, hash_t), void(*print)(node*)) {
	HashMap* hashMap = (HashMap*)malloc(sizeof(HashMap));
	if (hashMap == NULL) {
		return NULL;
	}
	hashMap->blocks = (hashBucket*)malloc(sizeof(hashBucket));
	if (hashMap->blocks == NULL) {
		return NULL;
	}
	hashMap->blocks = NULL;
	hashMap->comparator = compare_hash;
	hashMap->f_hash = f_hash;
	hashMap->print = print;
	return hashMap;
}



void putToMap(HashMap* map, void* data) {
	node* newNode = (node*)malloc(sizeof(node));
	if (newNode == NULL) {
		printf("Brak pamiêci");
		return;
	}
	newNode->k = map->f_hash(data);
	newNode->data = data;
	newNode->next = NULL;
	newNode->prev = NULL;
	
	hashBucket* newBucket = (hashBucket*)malloc(sizeof(hashBucket));
	if (newBucket == NULL) {
		printf("Brak pamiêci");
		return;
	}
	newBucket->beginningOfList = (node*)malloc(sizeof(node));
	if (newBucket->beginningOfList == NULL) {
		printf("Brak pamiêci");
		return;
	}
	newBucket->bucketNext = NULL;
	newBucket->bucketPrev = NULL;

	int index = map->f_hash(data);
	
	if (map->blocks == NULL) {
		newBucket->beginningOfList = newNode;
		map->blocks = newBucket;
	}
	else {
		hashBucket* temp = map->blocks;
		while (temp->bucketNext != NULL) {
			temp = temp->bucketNext;
			if (temp->beginningOfList->k == newNode->k) {
				node* tmpList = temp->beginningOfList;
				while (tmpList != NULL) {
					tmpList = tmpList->next;
				}
				tmpList = newNode;
			}
		}
		newBucket->beginningOfList = newNode;
		temp->bucketNext = newBucket;
		newBucket->bucketPrev = temp;
		
	}
}


node* getFromMap(HashMap* map, void* data) {
	hash_t k = map->f_hash(data);
	
	int index = map->f_hash(data);
	hashBucket* tmp = map->blocks;
	while (tmp != NULL) {
		
			if (map->comparator(tmp->beginningOfList->k, k)) {
				node* tmpList = tmp->beginningOfList;
				if (tmpList->next != NULL) {
					while (tmpList != NULL) {
						tmpList = tmpList->next;
					}
					return tmpList->prev;
				}
				return tmpList;
			}
		tmp = tmp->bucketNext;
		
		
	}
	return NULL;
}

void removeFromMap(HashMap* map, void* data) {
	hash_t k = map->f_hash(data);
	

	int index = map->f_hash(data);
	hashBucket* tmp = map->blocks;
	while (tmp != NULL) {
		if (map->comparator(tmp->beginningOfList->k, k)) {
			if (tmp->beginningOfList->next != NULL) {
				node* tmpList = tmp->beginningOfList;
				while (tmpList != NULL) {
					node* ptr = tmpList->next;
					free(tmpList);
					tmpList = tmpList->next;
				}
			}
			if (tmp->bucketPrev != NULL) {
				tmp->bucketPrev->bucketNext = tmp->bucketNext;
			}
			else {
				map->blocks = tmp->bucketNext;
			}
			if (tmp->bucketNext != NULL) {
				tmp->bucketNext->bucketPrev = tmp->bucketPrev;
			}
			free(tmp);
			return;
		}
		tmp = tmp->bucketNext;
	}
	
	map->blocks = tmp->bucketNext;
	
}

void deleteHashMap(HashMap* map)
{
	hashBucket* ptr = map->blocks;
	while (ptr != NULL) {
		if (ptr->beginningOfList != NULL) {
			node* tmp = ptr->beginningOfList;
			while (tmp != NULL) {
				node* Tnext = tmp->next;
				free(tmp);
				tmp = Tnext;
			}
		}
		hashBucket* tmp = ptr;
		ptr = ptr->bucketNext;
		free(tmp);
		
	}
	free(map);
	
}
