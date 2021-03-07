#include "utilities.h"
#include "../dcryptions/decryptionOMP.h"

char* readStringFromFile(FILE* fp, size_t allocated_size, int* input_length)
{
    char* string;
    int ch;
    *input_length = 0;
    string = (char*)realloc(NULL, sizeof(char) * allocated_size);
    if (!string)
        return string;
    while (EOF != (ch = fgetc(fp)))
    {
        if (ch == EOF)
            break;
        string[*input_length] = ch;
        *input_length += 1;
        if (*input_length == allocated_size) 
        {
            string = (char*)realloc(string, sizeof(char) * (allocated_size += EXTEND_SIZE));
            if (!string)
                return string;
        }
    }
    return (char*)realloc(string, sizeof(char) * (*input_length));
}

// Compare the word (hashed) with the hash table entries.
int compare(char *words, GHashTable *wordSet) {
    // Personal note - https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    int i, match = 0;
    const char s[3] = " \n";
    char *token, *tempWords = strdup(words);

    /* get the first token */
    token = strtok(tempWords, s);

    /* walk through other tokens */
    while (token != NULL) {
        if (g_hash_table_contains(wordSet, token)){
		match++;
	}
        token = strtok(NULL, s);
    }
    return match;
}

Session* decrypt(int keyStart, int keyStop, int keyLength, char* cipherText, size_t cipherLength, GHashTable* wordsGHash) {
   Session *session;

   // Delegate omp tasks to calculate half of all possible encryption keys each
   session = bruteForceOMP(keyStart, keyStop, keyLength, cipherText, cipherLength, wordsGHash);

   // Return null if decryption was unsuccessful
   if (!session->key) {
      return NULL;
   }

   // If one of the tasks succeeds return the session;
   if (session) {
      return session;
   }
   return NULL;
}

GHashTable* convertToHashTable(char* words) {
    // [Personal note] https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    char* token;
    GHashTable* hashTable = g_hash_table_new(g_int_hash, g_int_equal);

    token = strtok(words, "\n");

    while (token != NULL) {
        g_hash_table_add(hashTable, token);
        token = strtok(NULL, SEPERATOR);
    }
    return hashTable;
}

// Transform a decimal to an binary String representation.
char* decToBinary(int dec, int keyLength) {
    int count = 0, i;
    char *binaryString = (char*)malloc(sizeof(char)*(keyLength+1));

    // First fill the entire string with zeros.
    for (i = 0; i < keyLength; i++) {
        binaryString[i] = '0';
    }
    binaryString[keyLength] = '\0';
    // if the nuber decimal is 0 return the created binary.
    if (dec == 0) {
        return binaryString;
    }

    // The logic is mod & divition by 2. 
    while (dec > 0) {
        binaryString[keyLength - 1 - count] = dec % 2 + '0';
        dec = dec / 2;
        count++;
    }
    return binaryString;
}

void encryptText(char* key, int keyLength, char* plainText, size_t plainTextLength)
{
    char* encrypted;
    FILE* f = fopen("cipherText.e", "w");
    encrypted = cipher(key, keyLength, plainText, plainTextLength);
    fwrite(encrypted, sizeof(char), plainTextLength, f);
    fclose(f);
}
