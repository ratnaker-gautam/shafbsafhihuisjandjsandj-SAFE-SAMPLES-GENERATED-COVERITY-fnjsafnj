//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    char words[MAX_WORDS][MAX_WORD_LEN + 1] = {{0}};
    int word_count = 0;
    int in_word = 0;
    int word_start = 0;
    
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
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && !IS_WHITESPACE(input[i]) && !IS_TERMINATOR(input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                int word_len = i - word_start;
                if (word_len > MAX_WORD_LEN) {
                    fprintf(stderr, "Word too long\n");
                    return 1;
                }
                if (word_count >= MAX_WORDS) {
                    fprintf(stderr, "Too many words\n");
                    return 1;
                }
                strncpy(words[word_count], &input[word_start], word_len);
                words[word_count][word_len] = '\0';
                word_count++;
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len == 0) continue;
        
        char processed[MAX_WORD_LEN + 1] = {0};
        for (size_t j = 0; j < len; j++) {
            if (j == 0) {
                processed[j] = TO_UPPER(words[i][j]);
            } else {
                processed[j] = TO_LOWER(words[i][j]);
            }
        }
        processed[len] = '\0';
        
        printf("Word %d: %s -> %s\n", i + 1, words[i], processed);
    }
    
    printf("\nWord statistics:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    for (int i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
    }
    printf("Total characters: %d\n", total_chars);
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (float)total_chars / word_count);
    }
    
    return 0;
}