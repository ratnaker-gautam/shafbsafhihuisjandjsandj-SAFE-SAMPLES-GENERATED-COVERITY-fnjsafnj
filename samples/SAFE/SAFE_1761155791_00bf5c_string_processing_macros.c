//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

int split_into_words(const char *input, char words[][MAX_WORD_LEN]) {
    int word_count = 0;
    size_t i = 0;
    size_t input_len = strlen(input);
    
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        size_t word_start = i;
        size_t word_len = 0;
        
        while (i < input_len && !IS_WHITESPACE(input[i]) && word_len < MAX_WORD_LEN - 1) {
            word_len++;
            i++;
        }
        
        if (word_len > 0) {
            size_t j;
            for (j = 0; j < word_len; j++) {
                words[word_count][j] = input[word_start + j];
            }
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void process_word(char *word) {
    size_t len = strlen(word);
    if (len == 0) {
        return;
    }
    
    size_t i;
    for (i = 0; i < len; i++) {
        if (i == 0) {
            word[i] = TO_UPPER(word[i]);
        } else {
            word[i] = TO_LOWER(word[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("Original words: ");
    int i;
    for (i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("Processed words: ");
    for (i = 0; i < word_count; i++) {
        process_word(words[i]);
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}