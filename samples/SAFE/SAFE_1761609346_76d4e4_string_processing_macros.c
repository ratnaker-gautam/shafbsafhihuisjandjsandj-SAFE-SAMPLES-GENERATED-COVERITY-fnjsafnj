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
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (IS_WHITESPACE(input[i])) {
            if (in_word) {
                if (word_count < MAX_WORDS - 1) {
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
    
    if (in_word && word_count < MAX_WORDS) {
        words[word_count][pos] = '\0';
        word_count++;
    }
    
    return word_count;
}

void capitalize_words(char words[][MAX_WORD_LEN], int count) {
    for (int i = 0; i < count; i++) {
        if (words[i][0] != '\0') {
            words[i][0] = TO_UPPER(words[i][0]);
            for (int j = 1; words[i][j] != '\0'; j++) {
                words[i][j] = TO_LOWER(words[i][j]);
            }
        }
    }
}

void reverse_words(char words[][MAX_WORD_LEN], int count) {
    for (int i = 0; i < count / 2; i++) {
        char temp[MAX_WORD_LEN];
        if (strlen(words[i]) < MAX_WORD_LEN && strlen(words[count - i - 1]) < MAX_WORD_LEN) {
            strcpy(temp, words[i]);
            strcpy(words[i], words[count - i - 1]);
            strcpy(words[count - i - 1], temp);
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nOriginal words (%d):\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    capitalize_words(words, word_count);
    printf("\nCapitalized words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    reverse_words(words, word_count);
    printf("\nReversed order:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}