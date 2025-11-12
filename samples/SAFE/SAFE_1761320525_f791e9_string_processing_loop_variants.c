//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    int i, j, k;
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input string\n");
        return 0;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
        }
    }
    
    int alnum_count = j;
    processed[j] = '\0';
    
    printf("Alphanumeric characters (uppercase): %s\n", processed);
    printf("Count: %d\n", alnum_count);
    
    char reversed[MAX_LEN];
    k = 0;
    i = len - 1;
    while (i >= 0) {
        reversed[k++] = input[i];
        i--;
    }
    reversed[k] = '\0';
    
    printf("Reversed string: %s\n", reversed);
    
    int word_count = 0;
    int in_word = 0;
    
    for (i = 0; i < len; i++) {
        if (isspace((unsigned char)input[i])) {
            in_word = 0;
        } else {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        }
    }
    
    printf("Word count: %d\n", word_count);
    
    char no_vowels[MAX_LEN];
    j = 0;
    i = 0;
    do {
        char c = input[i];
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            no_vowels[j++] = c;
        }
        i++;
    } while (i < len);
    no_vowels[j] = '\0';
    
    printf("Without vowels: %s\n", no_vowels);
    
    return 0;
}