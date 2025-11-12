//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *text, size_t text_len, char words[MAX_WORDS][MAX_INPUT_LEN], size_t *word_count) {
    if (text == NULL || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        if (i < text_len && isalpha(text[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len > 0 && *word_count < MAX_WORDS) {
                    if (word_len >= MAX_INPUT_LEN) return 0;
                    memcpy(words[*word_count], &text[word_start], word_len);
                    words[*word_count][word_len] = '\0';
                    (*word_count)++;
                }
                in_word = 0;
            }
        }
    }
    return 1;
}

void sort_words(char words[MAX_WORDS][MAX_INPUT_LEN], size_t word_count) {
    if (words == NULL || word_count == 0) return;
    
    for (size_t i = 0; i < word_count - 1; i++) {
        for (size_t j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_INPUT_LEN];
                strcpy(temp, words[j]);
                strcpy(words[j], words[j + 1]);
                strcpy(words[j + 1], temp);
            }
        }
    }
}

int count_word_frequency(const char words[MAX_WORDS][MAX_INPUT_LEN], size_t word_count, 
                        const char *target, size_t *frequency) {
    if (words == NULL || target == NULL || frequency == NULL) return 0;
    
    *frequency = 0;
    for (size_t i = 0; i < word_count; i++) {
        if (strcmp(words[i], target) == 0) {
            (*frequency)++;
        }
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    size_t word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
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
    
    if (!extract_words(input, input_len, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nSorted words and frequencies:\n");
    size_t processed = 0;
    for (size_t i = 0; i < word_count; i++) {
        if (i > 0 && strcmp(words[i], words[i - 1]) == 0) {
            continue;
        }
        
        size_t freq;
        if (!count_word_frequency(words, word_count, words[i], &freq)) {
            fprintf(stderr, "Error counting frequency\n");
            return 1;
        }
        
        printf("%s: %zu\n", words[i], freq);
        processed++;
        
        if (processed >= 100) break;
    }
    
    printf("\nTotal unique words: %zu\n", processed);
    printf("Total words processed: %zu\n", word_count);
    
    return 0;
}