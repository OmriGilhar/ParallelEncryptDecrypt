#include <mpi.h>
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <glib.h>

#include "./../util/utilities.h"

#ifndef MASTER
	#define MASTER 0
#endif // !MASTER

#ifndef MAX_TEXT_LENGTH
	#define MAX_TEXT_SIZE 4096
#endif // !MAX_TEXT_LENGTH


void checkNumOfProcesses(int size);
void checkCMDCommand(int argc);
void parseArgs(char* argv[], int* keyLength, FILE** cipherFile);
void readWordsFiles(int argc, char* argv[], char** words, int* wordsLength);
void calcKeyRange(int rank, int keyLength, int* keyStart, int* keyTopLimit, int* keyStop);
void printSessionResaults(Session* session);
void usage();
void checkCharAllocation(char* allocation);
