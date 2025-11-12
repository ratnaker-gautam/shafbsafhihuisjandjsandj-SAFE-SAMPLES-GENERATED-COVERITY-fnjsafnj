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
            processed[j] = toupper((unsigned char)input[i]);
            j++;
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        fprintf(stderr, "No alphabetic characters found\n");
        return 1;
    }
    
    size_t processed_len = j;
    char reversed[MAX_LEN];
    
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
    
    size_t char_count[26] = {0};
    size_t n = 0;
    while (n < processed_len) {
        if (processed[n] >= 'A' && processed[n] <= 'Z') {
            char_count[processed[n] - 'A']++;
        }
        n++;
    }
    
    printf("Character frequencies:\n");
    for (int c = 0; c < 26; c++) {
        if (char_count[c] > 0) {
            printf("%c: %zu\n", 'A' + c, char_count[c]);
        }
    }
    
    return 0;
}