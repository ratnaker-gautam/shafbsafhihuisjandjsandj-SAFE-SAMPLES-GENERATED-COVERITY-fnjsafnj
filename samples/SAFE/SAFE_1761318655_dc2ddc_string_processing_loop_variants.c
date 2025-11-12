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
        printf("Empty string entered\n");
        return 0;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            if (j < MAX_LEN - 1) {
                processed[j++] = input[i];
            }
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
    
    i = 0;
    while (processed[i] != '\0') {
        char c = tolower((unsigned char)processed[i]);
        if (isdigit((unsigned char)processed[i])) {
            digit_count++;
        } else if (isalpha((unsigned char)processed[i])) {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
        i++;
    }
    
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    printf("Digits: %d\n", digit_count);
    
    char reversed[MAX_LEN];
    int k = 0;
    int m = j - 1;
    
    do {
        if (m >= 0) {
            reversed[k++] = processed[m--];
        }
    } while (m >= 0);
    reversed[k] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int palindrome = 1;
    int left = 0;
    int right = j - 1;
    
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