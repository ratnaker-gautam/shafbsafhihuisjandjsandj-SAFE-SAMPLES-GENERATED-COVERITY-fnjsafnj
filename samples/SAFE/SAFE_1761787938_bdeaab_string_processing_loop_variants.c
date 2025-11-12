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
    
    printf("Alphanumeric characters: %s\n", processed);
    
    int vowel_count = 0;
    int consonant_count = 0;
    char *ptr = processed;
    while (*ptr) {
        char c = tolower((unsigned char)*ptr);
        if (c >= 'a' && c <= 'z') {
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
    
    int digit_sum = 0;
    int k = 0;
    do {
        if (k < len && isdigit((unsigned char)input[k])) {
            digit_sum += input[k] - '0';
        }
        k++;
    } while (k < len);
    
    printf("Sum of digits: %d\n", digit_sum);
    
    char reversed[MAX_LENGTH];
    int m, n;
    for (m = len - 1, n = 0; m >= 0; m--, n++) {
        reversed[n] = input[m];
    }
    reversed[n] = '\0';
    
    printf("Reversed string: %s\n", reversed);
    
    int word_count = 0;
    int in_word = 0;
    for (int p = 0; p < len; p++) {
        if (isspace((unsigned char)input[p])) {
            in_word = 0;
        } else {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        }
    }
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}