//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
    int i, j, k;
    
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
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
        }
    }
    
    int alnum_count = j;
    
    for (k = 0; k < alnum_count; k++) {
        processed[j++] = processed[alnum_count - k - 1];
    }
    processed[j] = '\0';
    
    char temp;
    int palindrome_len = j;
    for (i = 0; i < palindrome_len / 2; i++) {
        temp = processed[i];
        processed[i] = processed[palindrome_len - i - 1];
        processed[palindrome_len - i - 1] = temp;
    }
    
    int word_count = 0;
    i = 0;
    while (i < len) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        if (i < len) {
            word_count++;
            while (i < len && !isspace((unsigned char)input[i])) {
                i++;
            }
        }
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Word count: %d\n", word_count);
    printf("Alphanumeric characters: %d\n", alnum_count);
    
    int digit_sum = 0;
    for (i = 0; i < len; i++) {
        if (isdigit((unsigned char)input[i])) {
            digit_sum += (input[i] - '0');
        }
    }
    printf("Digit sum: %d\n", digit_sum);
    
    return 0;
}