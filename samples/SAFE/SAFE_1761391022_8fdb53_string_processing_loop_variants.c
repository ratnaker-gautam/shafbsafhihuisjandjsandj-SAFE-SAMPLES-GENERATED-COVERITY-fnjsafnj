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
        if (isalpha((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
        }
    }
    processed[j] = '\0';
    
    if (j == 0) {
        fprintf(stderr, "No alphabetic characters found\n");
        return 1;
    }
    
    size_t processed_len = j;
    size_t vowel_count = 0;
    size_t k = 0;
    while (k < processed_len) {
        char c = processed[k];
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            vowel_count++;
        }
        k++;
    }
    
    char reversed[MAX_LENGTH];
    size_t m = 0;
    size_t n = processed_len;
    do {
        if (n > 0) {
            reversed[m++] = processed[n - 1];
            n--;
        }
    } while (n > 0);
    reversed[m] = '\0';
    
    printf("Original: %s\n", input);
    printf("Uppercase letters only: %s\n", processed);
    printf("Vowel count: %zu\n", vowel_count);
    printf("Reversed: %s\n", reversed);
    
    size_t palindrome = 1;
    size_t left = 0;
    size_t right = processed_len - 1;
    while (left < right) {
        if (processed[left] != processed[right]) {
            palindrome = 0;
            break;
        }
        left++;
        right--;
    }
    
    if (palindrome) {
        printf("The processed string is a palindrome\n");
    } else {
        printf("The processed string is not a palindrome\n");
    }
    
    return 0;
}