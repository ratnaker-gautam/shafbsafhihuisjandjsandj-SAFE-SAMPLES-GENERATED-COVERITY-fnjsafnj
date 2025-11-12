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
    
    printf("Enter a string to process: ");
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
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: %s\n", processed);
    
    int vowel_count = 0;
    int consonant_count = 0;
    int digit_count = 0;
    
    char *ptr = processed;
    while (*ptr != '\0') {
        char c = tolower((unsigned char)*ptr);
        if (c >= '0' && c <= '9') {
            digit_count++;
        } else if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowel_count++;
        } else if (c >= 'a' && c <= 'z') {
            consonant_count++;
        }
        ptr++;
    }
    
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    printf("Digits: %d\n", digit_count);
    
    int processed_len = strlen(processed);
    if (processed_len > 1) {
        char reversed[MAX_LENGTH];
        int k = processed_len - 1;
        int l = 0;
        
        do {
            reversed[l++] = processed[k--];
        } while (k >= 0);
        reversed[l] = '\0';
        
        printf("Reversed: %s\n", reversed);
        
        int is_palindrome = 1;
        int m = 0;
        int n = processed_len - 1;
        
        while (m < n) {
            if (processed[m] != processed[n]) {
                is_palindrome = 0;
                break;
            }
            m++;
            n--;
        }
        
        printf("Is palindrome: %s\n", is_palindrome ? "Yes" : "No");
    } else {
        printf("Reversed: %s\n", processed);
        printf("Is palindrome: Yes\n");
    }
    
    return 0;
}