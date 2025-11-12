//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        int word_start = i;
        while (i < input_len && !IS_WHITESPACE(input[i])) {
            i++;
        }
        
        int word_len = i - word_start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char current_word[MAX_WORD_LEN];
        int j;
        for (j = 0; j < word_len; j++) {
            current_word[j] = TO_LOWER(input[word_start + j]);
        }
        current_word[j] = '\0';
        
        int k;
        for (k = 0; k < j; k++) {
            if (!IS_ALPHA(current_word[k])) {
                break;
            }
        }
        
        if (k < j) {
            continue;
        }
        
        int found = 0;
        for (int w = 0; w < word_count; w++) {
            if (strcmp(words[w].word, current_word) == 0) {
                words[w].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (word_count < MAX_WORDS) {
                strncpy(words[word_count].word, current_word, sizeof(words[word_count].word) - 1);
                words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    printf("\nWord frequency analysis:\n");
    for (int w = 0; w < word_count; w++) {
        printf("%s: %d\n", words[w].word, words[w].count);
    }
    
    return 0;
}