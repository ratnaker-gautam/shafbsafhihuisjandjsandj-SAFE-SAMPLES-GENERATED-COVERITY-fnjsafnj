//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int split_words(const char *str, char words[][32], int *word_count) {
    if (str == NULL || words == NULL || word_count == NULL) return 0;
    *word_count = 0;
    size_t len = strlen(str);
    if (len == 0) return 1;
    
    int in_word = 0;
    size_t word_len = 0;
    
    for (size_t i = 0; i <= len; i++) {
        if (i < len && !isspace((unsigned char)str[i])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < 31) {
                words[*word_count][word_len++] = str[i];
            }
        } else {
            if (in_word) {
                if (word_len > 0) {
                    words[*word_count][word_len] = '\0';
                    (*word_count)++;
                    if (*word_count >= MAX_WORDS) break;
                }
                in_word = 0;
            }
        }
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int compare_strings(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    int word_count = 0;
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    if (!split_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
    
    qsort(words, word_count, sizeof(words[0]), compare_strings);
    
    printf("Sorted words (case-insensitive):\n");
    for (int i = 0; i < word_count; i++) {
        if (printf("%s\n", words[i]) < 0) {
            fprintf(stderr, "Error writing output\n");
            return 1;
        }
    }
    
    printf("Total words processed: %d\n", word_count);
    
    return 0;
}