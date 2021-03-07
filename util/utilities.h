#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glib.h>

#define START_SIZE 512
#define EXTEND_SIZE 32
#define MIN_KEY_LENGTH 4
#define SEPERATOR "\n"
#define DEFAULT_WORDS_FILE "/usr/share/dict/words"
#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */

#include "session.h"

char* readStringFromFile(FILE* fp, size_t allocated_size, int* input_length);

int compare(char *input, GHashTable *table);
Session* decrypt(int keyStart, int keyStop, int keyLength, char* cipherText, size_t cipherLength, GHashTable* wordsGHash);
char* decToBinary(int n, int keyLen);
GHashTable *convertToHashTable(char* words);
void encryptText(char* key, int keyLength, char* plainText, size_t plainTextLength);
