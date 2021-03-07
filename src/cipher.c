#include "cipher.h"


void binaryStringToBinary(char *string, size_t num_bytes)
{
    int i, byte;
    unsigned char binary_key[num_bytes];
    for(byte = 0; byte < num_bytes; byte++)
    {
        binary_key[byte] = 0;
        for(i = 0; i < 8; i++)
        {
            binary_key[byte] = binary_key[byte] << 1;
            binary_key[byte] |= string[byte * 8 + i] == '1' ? 1 : 0;  
        }
    }
    memcpy(string, binary_key, num_bytes);
}

char* cipher(char *key, size_t key_len, char *input, size_t inputLength)
{
    int i, j = 0;
    char *output_str = (char*)malloc(inputLength * sizeof(char));

    if (!input || !output_str)
    {
        fprintf(stderr, "Error reading string\n");
        exit(0);
    }
    for (i = 0; i < inputLength; i++, j++)
    {
        if (j == key_len) {
            j = 0;
        }
        output_str[i] = input[i] ^ key[j];
    }
    return output_str;
}

