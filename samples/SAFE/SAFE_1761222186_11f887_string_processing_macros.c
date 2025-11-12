//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

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
    
    printf("\nProcessed words (%d found):\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nWord frequency analysis:\n");
    int processed[MAX_WORDS] = {0};
    
    for (i = 0; i < word_count; i++) {
        if (processed[i]) {
            continue;
        }
        
        int count = 1;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s': %d occurrence%s\n", words[i], count, count == 1 ? "" : "s");
        processed[i] = 1;
    }
    
    char longest_word[MAX_WORD_LEN] = "";
    char shortest_word[MAX_WORD_LEN] = "";
    size_t min_len = MAX_WORD_LEN;
    size_t max_len = 0;
    
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
            strncpy(longest_word, words[i], MAX_WORD_LEN - 1);
            longest_word[MAX_WORD_LEN - 1] = '\0';
        }
        if (len < min_len && len > 0) {
            min_len = len;
            strncpy(shortest_word, words[i], MAX_WORD_LEN - 1);
            shortest_word[MAX_WORD_LEN - 1] = '\0';
        }
    }
    
    printf("\nLongest word: '%s' (%zu characters)\n", longest_word, max_len);
    printf("Shortest word: '%s' (%zu characters)\n", shortest_word, min_len);
    
    return 0;
}