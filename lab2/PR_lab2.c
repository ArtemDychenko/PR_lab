#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "hashMap.h"
#include "types.h"




void print_function(node* node) {
    printf("Key: %d, Data: %s\n", node->k, (char *)node->data);
}

hash_t hash_function(void* key) {
    char* str = (char*)key;
    hash_t hash = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hash = hash * 31 + str[i]; 
    }
    return hash;
}

int compare_function(hash_t key1, hash_t key2) {
    return key1 == key2 ? 1 : 0;
}



int main()
{
    HashMap* hashMap = init_hash_map(hash_function, compare_function, print_function);
    if (hashMap == NULL) {
        printf("Brak pamiêci");
        return 0;
    }

    // Dodanie elementów do mapy
    char* str = (char*)malloc(20 * sizeof(char));
    strcpy(str, "John");
    char* str1 = (char*)malloc(20 * sizeof(char));
    strcpy(str1, "Anna");
    char* str2 = (char*)malloc(20 * sizeof(char));
    strcpy(str2, "Vien");
    char* str3 = (char*)malloc(20 * sizeof(char));
    strcpy(str3, "Sara");
    char* str4 = (char*)malloc(20 * sizeof(char));
    strcpy(str4, "Alex");
    char* str5 = (char*)malloc(20 * sizeof(char));
    strcpy(str5, "Jake");



    putToMap(hashMap, str);
    putToMap(hashMap, str1);
    putToMap(hashMap, str2);
    putToMap(hashMap, str3);
    putToMap(hashMap, str4);
    putToMap(hashMap, str5);
 

   

    removeFromMap(hashMap, str);
    removeFromMap(hashMap, str5);
   
    node* result = getFromMap(hashMap, str);
    if (result != NULL) {
        printf("Found: ");
        print_function(result);
    }
    else {
        printf("Not found\n");
    }
  

    result = getFromMap(hashMap, str1);
    if (result != NULL) {
        printf("Found: ");
        print_function(result);
    }
    else {
        printf("Not found\n");
    }

    result = getFromMap(hashMap, str2);
    if (result != NULL) {
        printf("Found: ");
        print_function(result);
    }
    else {
        printf("Not found\n");
    }

    result = getFromMap(hashMap, str3);
    if (result != NULL) {
        printf("Found: ");
        print_function(result);
    }
    else {
        printf("Not found\n");
    }
    
    result = getFromMap(hashMap, str4);
    if (result != NULL) {
        printf("Found: ");
        print_function(result);
    }
    else {
        printf("Not found\n");
    }

    result = getFromMap(hashMap, str5);
    if (result != NULL) {
        printf("Found: ");
        print_function(result);
    }
    else {
        printf("Not found\n");
    }

    deleteHashMap(hashMap);

    
    free(str);
    free(str1);
    free(str2);
    free(str3);
    free(str4);
    free(str5);
    

    return 0;
}
