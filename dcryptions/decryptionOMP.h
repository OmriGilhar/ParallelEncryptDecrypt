#pragma once

#include <omp.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "../util/utilities.h"
#include "../src/cipher.h"

#define MAX_TEXT_SIZE 512
#define NUMBER_OF_THREADS 4

Session* bruteForceOMP(int keyStart, int keyStop, int keyLength, char* cipherText, size_t cipherLength, GHashTable* wordsGHash);


