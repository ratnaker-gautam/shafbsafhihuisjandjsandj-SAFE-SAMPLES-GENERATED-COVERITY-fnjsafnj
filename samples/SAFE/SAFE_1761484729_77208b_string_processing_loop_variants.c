//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    
    printf("Enter a string: ");
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
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    if (len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = input[i];
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        fprintf(stderr, "No alphanumeric characters found\n");
        return 1;
    }
    
    char reversed[MAX_LENGTH];
    size_t k = 0;
    size_t rev_len = strlen(processed);
    
    while (rev_len > 0) {
        reversed[k++] = processed[rev_len - 1];
        rev_len--;
    }
    reversed[k] = '\0';
    
    char vowels_removed[MAX_LENGTH];
    size_t m = 0;
    size_t proc_len = strlen(processed);
    size_t n = 0;
    
    do {
        char c = processed[n];
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            vowels_removed[m++] = c;
        }
        n++;
    } while (n < proc_len);
    vowels_removed[m] = '\0';
    
    char uppercase[MAX_LENGTH];
    size_t p = 0;
    size_t vow_len = strlen(vowels_removed);
    
    for (size_t q = 0; q < vow_len; q++) {
        uppercase[p++] = toupper((unsigned char)vowels_removed[q]);
    }
    uppercase[p] = '\0';
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Vowels removed: %s\n", vowels_removed);
    printf("Uppercase: %s\n", uppercase);
    
    return 0;
}