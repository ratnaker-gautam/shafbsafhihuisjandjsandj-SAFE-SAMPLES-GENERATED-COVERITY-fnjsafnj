//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    int i, j, len;
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LENGTH) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = input[i];
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        printf("No alphanumeric characters found\n");
        return 0;
    }
    
    char reversed[MAX_LENGTH];
    int k = 0;
    int processed_len = strlen(processed);
    
    while (processed_len > 0) {
        reversed[k++] = processed[processed_len - 1];
        processed_len--;
    }
    reversed[k] = '\0';
    
    char vowels_removed[MAX_LENGTH];
    int m = 0;
    int n = 0;
    
    do {
        char c = reversed[n];
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            vowels_removed[m++] = c;
        }
        n++;
    } while (reversed[n] != '\0');
    vowels_removed[m] = '\0';
    
    char final[MAX_LENGTH];
    int p = 0;
    int q = 0;
    
    for (q = 0; vowels_removed[q] != '\0'; q++) {
        if (p < MAX_LENGTH - 1) {
            final[p++] = vowels_removed[q];
        }
    }
    final[p] = '\0';
    
    if (p == 0) {
        printf("Result is empty\n");
        return 0;
    }
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Vowels removed: %s\n", vowels_removed);
    printf("Final result: %s\n", final);
    
    return 0;
}