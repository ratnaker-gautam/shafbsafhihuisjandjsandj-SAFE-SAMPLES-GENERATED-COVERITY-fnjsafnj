//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN];
    int i, j, len;
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LEN - 1) {
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
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: %s\n", processed);
    
    int vowel_count = 0;
    int consonant_count = 0;
    int digit_count = 0;
    
    char *ptr = processed;
    while (*ptr) {
        char c = *ptr;
        if (isdigit((unsigned char)c)) {
            digit_count++;
        } else if (isalpha((unsigned char)c)) {
            c = tolower((unsigned char)c);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
        ptr++;
    }
    
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    printf("Digits: %d\n", digit_count);
    
    int k = 0;
    int processed_len = strlen(processed);
    char reversed[MAX_LEN];
    
    if (processed_len > 0) {
        for (k = processed_len - 1; k >= 0; k--) {
            reversed[processed_len - 1 - k] = processed[k];
        }
        reversed[processed_len] = '\0';
        printf("Reversed: %s\n", reversed);
    }
    
    int word_count = 0;
    int in_word = 0;
    
    for (i = 0; i < len; i++) {
        if (isspace((unsigned char)input[i])) {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
        }
    }
    
    printf("Words: %d\n", word_count);
    
    return 0;
}