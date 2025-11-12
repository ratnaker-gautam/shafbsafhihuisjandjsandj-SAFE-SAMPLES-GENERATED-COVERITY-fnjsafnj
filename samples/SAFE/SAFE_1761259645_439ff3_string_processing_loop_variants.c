//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN + 1];
    char processed[MAX_INPUT_LEN + 1];
    
    printf("Enter a string to process: ");
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
    
    size_t proc_idx = 0;
    size_t word_count = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            processed[proc_idx++] = toupper((unsigned char)input[i]);
        } else if (isspace((unsigned char)input[i])) {
            if (in_word) {
                processed[proc_idx++] = ' ';
                in_word = 0;
            }
        }
    }
    
    if (proc_idx > 0 && processed[proc_idx - 1] == ' ') {
        proc_idx--;
    }
    processed[proc_idx] = '\0';
    
    if (proc_idx == 0) {
        fprintf(stderr, "No alphabetic characters found\n");
        return 1;
    }
    
    printf("Original: \"%s\"\n", input);
    printf("Processed: \"%s\"\n", processed);
    printf("Word count: %zu\n", word_count);
    
    char reversed[MAX_INPUT_LEN + 1];
    size_t rev_idx = 0;
    size_t i = proc_idx;
    
    while (i > 0) {
        i--;
        reversed[rev_idx++] = processed[i];
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: \"%s\"\n", reversed);
    
    size_t char_count = 0;
    for (size_t j = 0; j < proc_idx; j++) {
        if (processed[j] != ' ') {
            char_count++;
        }
    }
    
    printf("Non-space characters: %zu\n", char_count);
    
    return 0;
}