//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    
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
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    if (len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        fprintf(stderr, "No alphanumeric characters found\n");
        return 1;
    }
    
    size_t processed_len = j;
    char reversed[MAX_LENGTH];
    
    size_t k = 0;
    while (k < processed_len) {
        reversed[k] = processed[processed_len - 1 - k];
        k++;
    }
    reversed[processed_len] = '\0';
    
    int is_palindrome = 1;
    size_t m = 0;
    do {
        if (processed[m] != reversed[m]) {
            is_palindrome = 0;
            break;
        }
        m++;
    } while (m < processed_len);
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    
    if (is_palindrome) {
        printf("The processed string is a palindrome\n");
    } else {
        printf("The processed string is not a palindrome\n");
    }
    
    size_t vowel_count = 0;
    size_t consonant_count = 0;
    for (size_t n = 0; n < processed_len; n++) {
        char c = processed[n];
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            vowel_count++;
        } else if (c >= 'A' && c <= 'Z') {
            consonant_count++;
        }
    }
    
    printf("Vowels: %zu\n", vowel_count);
    printf("Consonants: %zu\n", consonant_count);
    
    return 0;
}