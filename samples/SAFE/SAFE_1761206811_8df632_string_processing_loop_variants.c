//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT * 2];
    int i, j, k;
    
    printf("Enter text to process: ");
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
        printf("No input provided\n");
        return 1;
    }
    
    if (len >= MAX_INPUT - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalpha(input[i])) {
            processed[j++] = toupper(input[i]);
            processed[j++] = tolower(input[i]);
        } else if (isdigit(input[i])) {
            int digit = input[i] - '0';
            for (k = 0; k < digit && j < MAX_INPUT * 2 - 1; k++) {
                processed[j++] = '*';
            }
        } else {
            processed[j++] = input[i];
        }
        
        if (j >= MAX_INPUT * 2 - 1) {
            break;
        }
    }
    processed[j] = '\0';
    
    char reversed[MAX_INPUT * 2];
    int rev_idx = 0;
    int input_len = strlen(processed);
    
    for (i = input_len - 1; i >= 0; i--) {
        if (rev_idx < MAX_INPUT * 2 - 1) {
            reversed[rev_idx++] = processed[i];
        }
    }
    reversed[rev_idx] = '\0';
    
    int word_count = 0;
    int in_word = 0;
    for (i = 0; processed[i] != '\0'; i++) {
        if (isalpha(processed[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Word count: %d\n", word_count);
    
    return 0;
}