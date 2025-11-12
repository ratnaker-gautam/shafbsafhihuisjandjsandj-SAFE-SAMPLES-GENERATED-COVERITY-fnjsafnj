//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int split_words(const char *text, char words[][32], size_t max_words) {
    size_t word_count = 0;
    size_t word_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; text[i] != '\0'; i++) {
        if (isspace(text[i])) {
            if (in_word) {
                if (word_count < max_words && word_len < 31) {
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
                word_len = 0;
            }
        } else {
            if (!in_word) {
                in_word = 1;
            }
            if (word_len < 31) {
                words[word_count][word_len] = text[i];
                word_len++;
            }
        }
    }
    
    if (in_word && word_count < max_words && word_len < 31) {
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    return word_count;
}

void to_lowercase(char *str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int compare_strings(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    int word_count = split_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
    
    qsort(words, word_count, sizeof(words[0]), compare_strings);
    
    printf("Sorted unique words (case-insensitive):\n");
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i-1]) != 0) {
            printf("%s\n", words[i]);
        }
    }
    
    printf("Total words processed: %d\n", word_count);
    printf("Unique words: ");
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i-1]) != 0) {
            unique_count++;
        }
    }
    printf("%d\n", unique_count);
    
    return 0;
}