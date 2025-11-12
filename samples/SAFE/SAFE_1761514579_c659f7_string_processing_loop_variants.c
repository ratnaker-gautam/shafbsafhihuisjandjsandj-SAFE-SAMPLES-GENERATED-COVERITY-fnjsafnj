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
    
    int processed_len = j;
    if (processed_len == 0) {
        printf("No alphanumeric characters found\n");
        return 0;
    }
    
    char reversed[MAX_LENGTH];
    for (i = processed_len - 1, j = 0; i >= 0; i--, j++) {
        reversed[j] = processed[i];
    }
    reversed[j] = '\0';
    
    char vowels_removed[MAX_LENGTH];
    j = 0;
    i = 0;
    while (processed[i] != '\0') {
        char c = tolower((unsigned char)processed[i]);
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u') {
            vowels_removed[j++] = processed[i];
        }
        i++;
    }
    vowels_removed[j] = '\0';
    
    int digit_count = 0;
    i = 0;
    do {
        if (isdigit((unsigned char)processed[i])) {
            digit_count++;
        }
        i++;
    } while (processed[i] != '\0');
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Vowels removed: %s\n", vowels_removed);
    printf("Digit count: %d\n", digit_count);
    
    return 0;
}