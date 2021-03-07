#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../util/utilities.h"


char* readStringFromFile(FILE* fp, size_t allocated_size, int* input_length);
void binaryStringToBinary(char* string, size_t num_bytes);
char* cipher(char* key, size_t key_len, char* input, size_t inputLength);

