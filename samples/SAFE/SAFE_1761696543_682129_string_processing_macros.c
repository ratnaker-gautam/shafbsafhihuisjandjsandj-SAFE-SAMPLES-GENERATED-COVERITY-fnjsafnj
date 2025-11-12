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
    int pos = 0;
    int in_word = 0;
    
    for (int i = 0; input[i] != '\0' && word_count < MAX_WORDS; i++) {
        if (IS_WHITESPACE(input[i])) {
            if (in_word) {
                if (pos < MAX_WORD_LEN - 1) {
                    words[word_count][pos] = '\0';
                    word_count++;
                }
                in_word = 0;
                pos = 0;
            }
        } else {
            if (!in_word) {
                in_word = 1;
            }
            if (pos < MAX_WORD_LEN - 1) {
                words[word_count][pos] = input[i];
                pos++;
            }
        }
    }
    
    if (in_word && word_count < MAX_WORDS && pos < MAX_WORD_LEN - 1) {
        words[word_count][pos] = '\0';
        word_count++;
    }
    
    return word_count;
}

void process_word(char *word) {
    int len = 0;
    while (word[len] != '\0' && len < MAX_WORD_LEN - 1) {
        len++;
    }
    
    if (len == 0) return;
    
    for (int i = 0; i < len; i++) {
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
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\0')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words);
    
    if (word_count == 0) {
        fprintf(stderr, "No words found\n");
        return 1;
    }
    
    printf("Original words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    for (int i = 0; i < word_count; i++) {
        process_word(words[i]);
    }
    
    printf("Processed words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}