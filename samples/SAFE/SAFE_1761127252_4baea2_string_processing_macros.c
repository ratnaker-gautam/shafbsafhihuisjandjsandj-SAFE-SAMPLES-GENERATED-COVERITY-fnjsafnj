//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, MAX_INPUT_LEN + 1);
    return len > 0 && len <= MAX_INPUT_LEN;
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    while (src[i] != '\0' && i < max_len - 1) {
        if (isalpha((unsigned char)src[i])) {
            dest[i] = TO_LOWER(src[i]);
        } else {
            dest[i] = src[i];
        }
        i++;
    }
    dest[i] = '\0';
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_input(const char *input, WordCount *words, int max_words) {
    char buffer[MAX_INPUT_LEN + 1];
    char current_word[MAX_WORD_LEN];
    int word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    if (input_len >= sizeof(buffer)) return 0;
    
    strncpy(buffer, input, input_len);
    buffer[input_len] = '\0';
    
    size_t pos = 0;
    size_t word_start = 0;
    int in_word = 0;
    
    while (pos <= input_len && word_count < max_words) {
        char c = buffer[pos];
        
        if (isalpha((unsigned char)c) && !in_word) {
            word_start = pos;
            in_word = 1;
        } else if (!isalpha((unsigned char)c) && in_word) {
            size_t word_len = pos - word_start;
            if (word_len > 0 && word_len < MAX_WORD_LEN) {
                strncpy(current_word, &buffer[word_start], word_len);
                current_word[word_len] = '\0';
                normalize_word(current_word, current_word, MAX_WORD_LEN);
                
                if (safe_strlen(current_word, MAX_WORD_LEN) > 0) {
                    int idx = find_word_index(words, word_count, current_word);
                    if (idx >= 0) {
                        if (words[idx].count < INT_MAX) {
                            words[idx].count++;
                        }
                    } else {
                        strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
                        words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
            }
            in_word = 0;
        }
        
        pos++;
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    int word_count = process_input(input, words, MAX_WORDS);
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No words found in input.\n");
    }
    
    return EXIT_SUCCESS;
}