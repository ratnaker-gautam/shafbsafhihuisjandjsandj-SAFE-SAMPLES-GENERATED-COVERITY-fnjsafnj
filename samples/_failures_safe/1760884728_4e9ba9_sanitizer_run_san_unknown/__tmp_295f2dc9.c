//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN + 1];
    char processed[MAX_INPUT_LEN * 2 + 1];
    
    printf("Enter a string to process (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t processed_idx = 0;
    int word_count = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
                if (word_count > 1) {
                    if (processed_idx < sizeof(processed) - 1) {
                        processed[processed_idx++] = ' ';
                    }
                }
            }
            if (processed_idx < sizeof(processed) - 1) {
                processed[processed_idx++] = tolower((unsigned char)input[i]);
            }
        } else {
            in_word = 0;
        }
    }
    
    if (processed_idx == 0) {
        fprintf(stderr, "No alphabetic characters found\n");
        return 1;
    }
    
    processed[processed_idx] = '\0';
    
    printf("Original: \"%s\"\n", input);
    printf("Processed: \"%s\"\n", processed);
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_INPUT_LEN + 1];
    size_t reversed_len = 0;
    
    for (int i = (int)processed_idx - 1; i >= 0; i--) {
        if (reversed_len < sizeof(reversed) - 1) {
            reversed[reversed_len++] = processed[i];
        }
    }
    reversed[reversed_len] = '\0';
    
    printf("Reversed: \"%s\"\n", reversed);
    
    int palindrome = 1;
    for (size_t i = 0; i < processed_idx / 2; i++) {
        if (processed[i] != processed[processed_idx - 1 - i]) {
            palindrome = 0;
            break;
        }
    }
    
    printf("Is palindrome: %s\n", palindrome ? "yes" : "no");
    
    return 0;
}