//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (input[i] != '\0' && word_count < MAX_WORDS) {
        while (IS_WHITESPACE(input[i]) && input[i] != '\0') {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        j = 0;
        while (!IS_WHITESPACE(input[i]) && input[i] != '\0' && j < MAX_WORD_LEN - 1) {
            words[word_count][j] = TO_LOWER(input[i]);
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Processing %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        int is_palindrome = 1;
        size_t len = strlen(words[i]);
        
        if (len == 0) {
            continue;
        }
        
        for (j = 0, k = len - 1; j < k; j++, k--) {
            if (words[i][j] != words[i][k]) {
                is_palindrome = 0;
                break;
            }
        }
        
        printf("Word %d: \"%s\" (length: %zu) - %s\n", 
               i + 1, words[i], len, is_palindrome ? "palindrome" : "not palindrome");
    }
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += (int)strlen(words[i]);
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters (excluding spaces): %d\n", total_chars);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0);
    
    return 0;
}