//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a sentence: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && j < MAX_WORD_LEN - 1 && !IS_WHITESPACE(input[i])) {
            words[word_count][j] = input[i];
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
    
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        if (word_len == 0) {
            continue;
        }
        
        words[i][0] = TO_UPPER(words[i][0]);
        
        for (j = 1; j < word_len; j++) {
            words[i][j] = TO_LOWER(words[i][j]);
        }
        
        if (IS_TERMINATOR(words[i][word_len - 1])) {
            words[i][word_len - 1] = '\0';
        }
    }
    
    printf("Processed words:\n");
    for (i = 0; i < word_count; i++) {
        if (strlen(words[i]) > 0) {
            printf("%s\n", words[i]);
        }
    }
    
    char longest_word[MAX_WORD_LEN] = "";
    size_t max_len = 0;
    
    for (i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        if (current_len > max_len && current_len > 0) {
            max_len = current_len;
            strncpy(longest_word, words[i], sizeof(longest_word) - 1);
            longest_word[sizeof(longest_word) - 1] = '\0';
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: %s (%zu characters)\n", longest_word, max_len);
    }
    
    return 0;
}