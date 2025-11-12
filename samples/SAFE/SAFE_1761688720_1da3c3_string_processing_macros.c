//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) ((c) >= 'a' && (c) <= 'z' ? (c) - 32 : (c))
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (input[i] != '\0' && word_count < MAX_WORDS) {
        while (IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        j = 0;
        while (input[i] != '\0' && !IS_WHITESPACE(input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Processing %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        
        if (len == 0) {
            continue;
        }
        
        int has_letter = 0;
        for (j = 0; j < len; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                has_letter = 1;
                break;
            }
        }
        
        if (!has_letter) {
            printf("Non-word: %s\n", words[i]);
            continue;
        }
        
        char processed[MAX_WORD_LEN + 1];
        int write_pos = 0;
        int last_was_upper = 0;
        
        for (j = 0; j < len && write_pos < MAX_WORD_LEN; j++) {
            unsigned char c = words[i][j];
            
            if (isalpha(c)) {
                if (last_was_upper) {
                    processed[write_pos] = TO_LOWER(c);
                } else {
                    processed[write_pos] = TO_UPPER(c);
                }
                last_was_upper = !last_was_upper;
                write_pos++;
            } else if (isdigit(c)) {
                processed[write_pos] = c;
                write_pos++;
                last_was_upper = 0;
            } else if (IS_TERMINATOR(c)) {
                processed[write_pos] = c;
                write_pos++;
                last_was_upper = 0;
            }
        }
        
        processed[write_pos] = '\0';
        
        if (write_pos > 0) {
            printf("Original: %-15s Processed: %s\n", words[i], processed);
        }
    }
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}