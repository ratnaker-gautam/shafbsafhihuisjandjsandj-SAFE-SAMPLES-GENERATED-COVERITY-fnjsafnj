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
    
    int processed_len = j;
    
    if (processed_len == 0) {
        printf("No alphanumeric characters found\n");
        return 0;
    }
    
    j = processed_len;
    for (k = 0; k < processed_len; k++) {
        processed[j++] = processed[k];
    }
    
    processed[j] = '\0';
    
    int final_len = j;
    
    printf("Original: %s\n", input);
    printf("Processed: ");
    
    for (i = 0; i < final_len; i++) {
        putchar(processed[i]);
        if ((i + 1) % 10 == 0 && i != final_len - 1) {
            printf("-");
        }
    }
    printf("\n");
    
    int digit_count = 0;
    int letter_count = 0;
    i = 0;
    while (i < final_len) {
        if (isdigit((unsigned char)processed[i])) {
            digit_count++;
        } else if (isalpha((unsigned char)processed[i])) {
            letter_count++;
        }
        i++;
    }
    
    printf("Digits: %d, Letters: %d\n", digit_count, letter_count);
    
    char reversed[MAX_LENGTH * 2];
    for (i = final_len - 1, k = 0; i >= 0; i--, k++) {
        reversed[k] = processed[i];
    }
    reversed[k] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    return 0;
}