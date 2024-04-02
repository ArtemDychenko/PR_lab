#ifndef _HASHMAP_H_
#define _HASHMAP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"




hash_t f_hash(void* nd);

int compare_hash(hash_t, hash_t);

void print(node*);



HashMap* init_hash_map(hash_t(*f_hash)(void*), int(*compare_hash)(hash_t, hash_t), void(*print)(node*));

void putToMap(HashMap* map, void* data);

node* getFromMap(HashMap* map, void* data);

void removeFromMap(HashMap* map, void* data);

void deleteHashMap(HashMap* map);

#endif