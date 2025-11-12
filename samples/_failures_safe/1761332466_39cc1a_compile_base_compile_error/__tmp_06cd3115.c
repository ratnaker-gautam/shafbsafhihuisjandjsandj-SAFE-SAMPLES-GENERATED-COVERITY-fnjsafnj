//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return -1;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 0;
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_input(const char *input, WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return;
    
    char temp_word[MAX_WORD_LEN];
    int buf_pos = 0;
    int word_pos = 0;
    
    while (buffer[buf_pos] != '\0' && *word_count < MAX_WORDS) {
        if (IS_WHITESPACE(buffer[buf_pos])) {
            if (word_pos > 0) {
                temp_word[word_pos] = '\0';
                
                char processed_word[MAX_WORD_LEN];
                for (int i = 0; i < word_pos && i < MAX_WORD_LEN - 1; i++) {
                    processed_word[i] = TO_LOWER(temp_word[i]);
                }
                processed_word[word_pos < MAX_WORD_LEN ? word_pos : MAX_WORD_LEN - 1] = '\0';
                
                int found_idx = find_word(words, *word_count, processed_word);
                if (found_idx >= 0) {
                    if (words[found_idx].count < INT_MAX) {
                        words[found_idx].count++;
                    }
                } else {
                    if (safe_strncpy(words[*word_count].word, processed_word, MAX_WORD_LEN) == 0) {
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                word_pos = 0;
            }
            buf_pos++;
            continue;
        }
        
        if (word_pos < MAX_WORD_LEN - 1) {
            temp_word[word_pos++] = buffer[buf_pos];
        }
        buf_pos++;
    }
    
    if (word_pos > 0 && *word_count < MAX_WORDS) {
        temp_word[word_pos] = '\0';
        
        char processed_word[MAX_WORD_LEN];
        for (int i = 0; i < word_pos && i < MAX_WORD_LEN - 1; i++) {
            processed_word[i] = TO_LOWER(temp_word[i]);
        }
        processed_word[word_pos < MAX_WORD_LEN ? word_pos : MAX_WORD_LEN - 1] = '\0';
        
        int found_idx = find_word(words, *word_count, processed_word);
        if (found_idx >= 0) {
            if (words[found_idx].count < INT_MAX) {
                words[found_idx].count++;
            }
        } else {
            if (safe_strncpy(words[*word_count].word, processed_word, MAX_WORD_LEN) == 0) {
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
}

void print_results(WordCount *words, int word_count) {
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 0;
    }
    
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].word[0] = '\0';
        words[i].count = 0;
    }
    
    process_input(input, words, &word_count);
    
    if (word_count > 0) {
        print_results(words, word_count);