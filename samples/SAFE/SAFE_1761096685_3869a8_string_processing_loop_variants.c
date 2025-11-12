//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT * 2];
    int counts[26] = {0};
    
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
    
    size_t proc_idx = 0;
    for (size_t i = 0; i < len; i++) {
        char c = input[i];
        if (isalpha((unsigned char)c)) {
            char upper = toupper((unsigned char)c);
            processed[proc_idx++] = upper;
            if (upper >= 'A' && upper <= 'Z') {
                counts[upper - 'A']++;
            }
        } else if (isspace((unsigned char)c)) {
            processed[proc_idx++] = ' ';
        }
    }
    processed[proc_idx] = '\0';
    
    printf("Processed text: %s\n", processed);
    
    printf("Letter frequencies:\n");
    for (int i = 0; i < 26; i++) {
        if (counts[i] > 0) {
            printf("%c: %d\n", 'A' + i, counts[i]);
        }
    }
    
    char reversed[MAX_INPUT * 2];
    size_t rev_idx = 0;
    size_t proc_len = strlen(processed);
    if (proc_len > 0) {
        for (size_t i = proc_len; i > 0; i--) {
            reversed[rev_idx++] = processed[i - 1];
        }
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int word_count = 0;
    int in_word = 0;
    for (size_t i = 0; i < proc_len; i++) {
        if (processed[i] != ' ') {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}