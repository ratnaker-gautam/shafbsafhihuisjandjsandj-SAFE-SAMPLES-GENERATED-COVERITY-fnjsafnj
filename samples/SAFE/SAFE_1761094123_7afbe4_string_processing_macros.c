//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
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

void process_string(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return;
    
    int idx = 0;
    int in_word = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (buffer[idx] != '\0' && *word_count < MAX_WORDS) {
        if (IS_WHITESPACE(buffer[idx])) {
            if (in_word && word_len > 0) {
                current_word[word_len] = '\0';
                int found_idx = find_word(words, *word_count, current_word);
                if (found_idx >= 0) {
                    words[found_idx].count++;
                } else if (*word_count < MAX_WORDS) {
                    if (safe_strncpy(words[*word_count].word, current_word, MAX_WORD_LEN) == 0) {
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                word_len = 0;
            }
            in_word = 0;
        } else {
            if (!in_word) in_word = 1;
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = (char)TO_LOWER((unsigned char)buffer[idx]);
            }
        }
        idx++;
    }
    
    if (in_word && word_len > 0 && *word_count < MAX_WORDS) {
        current_word[word_len] = '\0';
        int found_idx = find_word(words, *word_count, current_word);
        if (found_idx >= 0) {
            words[found_idx].count++;
        } else {
            if (safe_strncpy(words[*word_count].word, current_word, MAX_WORD_LEN) == 0) {
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count != wb->count) return wb->count - wa->count;
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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    process_string(input, words, &word_count);
    
    if (word_count > 0) {
        qsort(words, (size_t)word_count, sizeof(WordCount), compare_word_count);
        printf("\nWord frequency analysis:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No words found in input\n");
    }
    
    return 0;
}