#include "decryptionOMP.h"

Session* bruteForceOMP(int keyStart, int keyStop, int keyLength, char* cipherText, size_t cipherLength, GHashTable *wordsGHash) {
	int bestCount = -1, matchs, i;
	char *key, *decrypted;

	// Init Session
	Session *session = (Session*) malloc(sizeof(Session));
	session->key = NULL;
	session->decryptedMessage = NULL;

	// Fixed number of thread for each process (4)
	omp_set_num_threads(NUMBER_OF_THREADS);

	#pragma omp parallel for
	for (i = keyStart; i < keyStop; i++) {
		
		// Get binary string representation fo key
		key = decToBinary(i, keyLength);

		// Try to decrypt the cipher with the current key
		decrypted = cipher(key, keyLength, cipherText, cipherLength);

		// Debug mode - Uncomman the next line to save encrypted file to use as an input
		// encryptText(key, keyLength, cipherText, cipherLength);
		
		// Check if the decrypted plaintext makes sense by matching it with the known words text
		matchs = compare(decrypted, wordsGHash);

		if (matchs > bestCount) {
			// Allocate memory for encryption key & plaintext
			session->key = (char*) malloc(sizeof(char) * keyLength);
			session->decryptedMessage = (char*) malloc(MAX_TEXT_SIZE * sizeof(char));

			// Save encryption key & plaintext
			strcpy(session->key, key);
			strcpy(session->decryptedMessage, decrypted);
			session->match = matchs;
			bestCount = matchs;
		}
	}
	return session;
}


