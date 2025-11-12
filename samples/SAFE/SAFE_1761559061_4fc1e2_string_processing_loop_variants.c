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
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
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
        fprintf(stderr, "Empty string provided\n");
        return 1;
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
        fprintf(stderr, "No alphanumeric characters found\n");
        return 1;
    }
    
    char reversed[MAX_LENGTH];
    int processed_len = strlen(processed);
    
    for (i = processed_len - 1; i >= 0; i--) {
        reversed[processed_len - 1 - i] = processed[i];
    }
    reversed[processed_len] = '\0';
    
    char vowels_removed[MAX_LENGTH];
    int vowel_count = 0;
    j = 0;
    i = 0;
    while (processed[i] != '\0') {
        char c = tolower((unsigned char)processed[i]);
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            vowels_removed[j++] = processed[i];
        } else {
            vowel_count++;
        }
        i++;
    }
    vowels_removed[j] = '\0';
    
    char uppercase[MAX_LENGTH];
    i = 0;
    do {
        uppercase[i] = toupper((unsigned char)processed[i]);
        i++;
    } while (processed[i] != '\0');
    uppercase[i] = '\0';
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Vowels removed: %s\n", vowels_removed);
    printf("Uppercase: %s\n", uppercase);
    printf("Vowels count: %d\n", vowel_count);
    
    int digit_count = 0;
    for (i = 0; processed[i] != '\0'; i++) {
        if (isdigit((unsigned char)processed[i])) {
            digit_count++;
        }
    }
    printf("Digit count: %d\n", digit_count);
    
    return 0;
}