//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))

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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int find_word(WordCount *words, int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) return i;
    }
    return -1;
}

void process_word(char *word) {
    if (word == NULL) return;
    size_t len = strlen(word);
    size_t write_pos = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)word[i])) {
            word[write_pos] = TO_LOWER(word[i]);
            write_pos++;
        }
    }
    word[write_pos] = '\0';
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char buffer[MAX_WORD_LEN];
    size_t buf_pos = 0;
    size_t input_pos = 0;
    
    while (input[input_pos] != '\0' && word_count < MAX_WORDS) {
        if (IS_WHITESPACE(input[input_pos])) {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                process_word(buffer);
                
                if (strlen(buffer) > 0) {
                    int found_idx = find_word(words, word_count, buffer);
                    if (found_idx >= 0) {
                        if (words[found_idx].count < INT_MAX) {
                            words[found_idx].count++;
                        }
                    } else {
                        if (safe_strncpy(words[word_count].word, buffer, MAX_WORD_LEN) == 0) {
                            words[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
                buf_pos = 0;
            }
        } else {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos] = input[input_pos];
                buf_pos++;
            }
        }
        input_pos++;
    }
    
    if (buf_pos > 0 && word_count < MAX_WORDS) {
        buffer[buf_pos] = '\0';
        process_word(buffer);
        
        if (strlen(buffer) > 0) {
            int found_idx = find_word(words, word_count, buffer);
            if (found_idx >= 0) {
                if (words[found_idx].count < INT_MAX) {
                    words[found_idx].count++;
                }
            } else {
                if (safe_strncpy(words[word_count].word, buffer, MAX_WORD_LEN) == 0) {
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}