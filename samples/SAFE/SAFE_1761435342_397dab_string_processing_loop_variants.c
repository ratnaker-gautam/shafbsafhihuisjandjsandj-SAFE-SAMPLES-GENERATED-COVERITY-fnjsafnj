//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN];
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
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
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            if (j < MAX_LEN - 1) {
                processed[j++] = toupper((unsigned char)input[i]);
            } else {
                break;
            }
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        fprintf(stderr, "No alphabetic characters found\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Uppercase letters only: %s\n", processed);
    
    size_t k = 0;
    size_t count = 0;
    while (processed[k] != '\0') {
        if (processed[k] == 'A' || processed[k] == 'E' || 
            processed[k] == 'I' || processed[k] == 'O' || 
            processed[k] == 'U') {
            count++;
        }
        k++;
    }
    printf("Vowel count: %zu\n", count);
    
    char reversed[MAX_LEN];
    size_t m = 0;
    size_t n = strlen(processed);
    if (n > 0) {
        do {
            if (m < MAX_LEN - 1) {
                reversed[m] = processed[n - 1 - m];
                m++;
            } else {
                break;
            }
        } while (m < n);
    }
    reversed[m] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    size_t freq[26] = {0};
    size_t p = 0;
    while (processed[p] != '\0') {
        if (processed[p] >= 'A' && processed[p] <= 'Z') {
            size_t idx = processed[p] - 'A';
            if (idx < 26) {
                freq[idx]++;
            }
        }
        p++;
    }
    
    printf("Letter frequencies:\n");
    for (size_t q = 0; q < 26; q++) {
        if (freq[q] > 0) {
            printf("%c: %zu\n", 'A' + q, freq[q]);
        }
    }
    
    return 0;
}