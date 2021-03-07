#include "main.h"

int main(int argc, char *argv[]) {
    // Premative
    int size, rank, cipherFileLength, wordsFileLength, keyStart, keyStop, keyTopLimit, keyLength;
    char *wordList, *cipherText;

    // Structs
    MPI_Status status;
    FILE *cipherFile, *wordsFile;
    GHashTable *wordsHashTable;
   
    // Custom structs
    Session* session;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    checkNumOfProcesses(size);
    checkCMDCommand(argc);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    if (rank == MASTER) {
        // Preparations of master environment.
        parseArgs(argv, &keyLength, &cipherFile);
        cipherText = readStringFromFile(cipherFile, MAX_TEXT_SIZE, &cipherFileLength);
        readWordsFiles(argc, argv, &wordList, &wordsFileLength);
        calcKeyRange(rank, keyLength, &keyStart, &keyTopLimit, &keyStop);
        
        // Message passing to slave.
        MPI_Send(&keyLength, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&keyTopLimit, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      
        MPI_Send(&cipherFileLength, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&wordsFileLength, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      
        MPI_Send(cipherText, cipherFileLength, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Send(wordList, wordsFileLength, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } 
    else {
        // Message reciving from master process.
        MPI_Recv(&keyLength, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&keyTopLimit, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&cipherFileLength, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&wordsFileLength, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        cipherText = (char*) malloc(sizeof(char) * cipherFileLength);
        checkCharAllocation(cipherText);
        
        wordList = (char*) malloc(sizeof(char) * wordsFileLength);
        checkCharAllocation(wordList);
        
        MPI_Recv(cipherText, cipherFileLength, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(wordList, wordsFileLength, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);

        // Preperation of slave environment.
        calcKeyRange(rank, keyLength, &keyStart, &keyTopLimit, &keyStop);
    }
    
    // Trasforming word list to HashTable for fast equivalent check
    wordsHashTable = convertToHashTable(wordList);
    
    // Send data to brute-force decryption (The actual logic)
    session = decrypt(keyStart, keyStop, keyLength, cipherText, cipherFileLength, wordsHashTable);
    
    if (rank != MASTER) {
        // Sanding slaves' session summary to master process.
        MPI_Send(session->decryptedMessage, MAX_TEXT_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        MPI_Send(session->key, MAX_TEXT_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&session->match, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        int matchs;

        // Allocating session for the slaves' session data.
        Session *slaveSession = (Session*)malloc(sizeof(Session));
      
        char *key = (char*) malloc(MAX_TEXT_SIZE);
        checkCharAllocation(key);
      
        char *plaintext = (char*) malloc(MAX_TEXT_SIZE);
        checkCharAllocation(plaintext);
      
        // Reciving slaves' session.
        MPI_Recv(plaintext, MAX_TEXT_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(key, MAX_TEXT_SIZE, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&matchs, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        
        slaveSession->decryptedMessage = plaintext;
        slaveSession->key = key;
        slaveSession->match = matchs;

        // Checking who has the best match for this brute force, it possible that one of the processes have decrypt word/s 
        // in accident, so we are checking the integrity of both sessions.
        if (session->match > slaveSession->match) {
            printSessionResaults(session);
        } else {
            printSessionResaults(slaveSession);
        }

        free(key);
        free(plaintext);
        free(slaveSession);
   }

   free(wordList);
   free(cipherText);
   free(session);
   g_hash_table_destroy(wordsHashTable);

   MPI_Finalize();

   return 0;
}

void checkNumOfProcesses(int size)
{
    // Check number of processes not greater then 2.
    if (size != 2) {
        printf(RED "[Error] - Please make sure you run this program with only 2 processes" RESET);
        printf(RED "[Error] - you have tried to run this program with %d processes." RESET, size);
        MPI_Abort(MPI_COMM_WORLD, __LINE__);
    }
}

void checkCMDCommand(int argc)
{
    // Check if there is enough agrs to run the program.
    if (argc < 3) {
        usage();
        MPI_Abort(MPI_COMM_WORLD, __LINE__);
    }
}

void usage()
{
    printf(RED "[Error] - Unable to parse the parameters\n" RESET);
    printf("[INFO] - Please run this program by the following format\n");
    printf("[INFO] - \tmpiexec -np 2 ./main size=<Key Length in number> encrypted=<path to the enctypted file>, words=[path to words file OPTIONAL]\n\n");
    printf("[INFO] - For more information please read the README.md\n");
}

void printSessionResaults(Session* session)
{
    printf("[INFO] - Plain Text: %s\n", session->decryptedMessage);
    printf("[INFO] - Encryption Key: %s\n", session->key);
    printf("[INFO] - Number of matches words: %d\n", session->match);
    printf(GREEN "[INFO] - Run successfully finished\n" RESET);
}

void parseArgs(char* argv[], int* keyLength, FILE **cipherFile)
{
    *keyLength = atoi(argv[1]);
    *cipherFile = fopen(argv[2], "r");
    // Check if cipher file exists.
    if (!*cipherFile)
    {
        fprintf(stderr, RED "[Error] - Could not open file: %s\n" RESET, argv[1]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

}

void readWordsFiles(int argc, char* argv[], char **words, int* wordsLength)
{
    FILE* wordsFile;

    // In case of word file in input.
    if (argc == 4) {
        wordsFile = fopen(argv[3], "r");
    }
    // In case there is no word file, take linux default file.
    else {
        wordsFile = fopen(DEFAULT_WORDS_FILE, "r");
    }

    // Read the input and the words from file.
    *words = readStringFromFile(wordsFile, MAX_TEXT_SIZE, wordsLength);
}

void calcKeyRange(int rank, int keyLength, int *keyStart, int *keyTopLimit, int *keyStop)
{
    *keyTopLimit = pow(2, keyLength);
    if (rank == MASTER)
    {
      *keyStart = 0;
      *keyStop = floor(*keyTopLimit / 2);
    } else {
      *keyStart = floor(*keyTopLimit / 2) + 1;
      *keyStop = *keyTopLimit;
    }
}

void checkCharAllocation(char *allocation)
{
    if (!allocation) {
        fprintf(stderr, RED "[Error] - Allocation filed.\n" RESET);
        MPI_Abort(MPI_COMM_WORLD, __LINE__);
    }
}



