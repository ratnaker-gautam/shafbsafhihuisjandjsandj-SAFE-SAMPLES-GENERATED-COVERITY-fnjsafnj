//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void normalize_word(char *word) {
    if (word == NULL || *word == '\0') return;
    size_t len = strlen(word);
    size_t write_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)word[i])) {
            word[write_idx++] = TO_LOWER(word[i]);
        }
    }
    word[write_idx] = '\0';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    *word_count = 0;
    char *token = buffer;
    int in_word = 0;
    size_t word_len = 0;
    char current_word[MAX_WORD_LEN];
    
    for (size_t i = 0; buffer[i] != '\0' && *word_count < MAX_WORDS; i++) {
        if (IS_WHITESPACE(buffer[i])) {
            if (in_word && word_len > 0 && word_len < MAX_WORD_LEN) {
                current_word[word_len] = '\0';
                normalize_word(current_word);
                if (current_word[0] != '\0') {
                    int found = 0;
                    for (int j = 0; j < *word_count; j++) {
                        if (strcmp(words[j].word, current_word) == 0) {
                            words[j].count++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        if (safe_strncpy(words[*word_count].word, current_word, MAX_WORD_LEN) == 0) {
                            words[*word_count].count = 1;
                            (*word_count)++;
                        }
                    }
                }
                in_word = 0;
                word_len = 0;
            }
        } else {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = buffer[i];
            }
        }
    }
    
    if (in_word && word_len > 0 && word_len < MAX_WORD_LEN) {
        current_word[word_len] = '\0';
        normalize_word(current_word);
        if (current_word[0] != '\0') {
            int found = 0;
            for (int j = 0; j < *word_count; j++) {
                if (strcmp(words[j].word, current_word) == 0) {
                    words[j].count++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                if (safe_strncpy(words[*word_count].word, current_word, MAX_WORD_LEN) == 0) {
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
    }
    
    return 0;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')