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

int is_valid_string(const char *str) {
    if (str == NULL) return 0;
    size_t len = 0;
    while (str[len] != '\0') {
        if (len >= MAX_INPUT_LEN) return 0;
        len++;
    }
    return 1;
}

int find_word(WordCount *words, int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    if (!is_valid_string(input)) return -1;
    
    int count = 0;
    size_t i = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (input[i] != '\0' && count < MAX_WORDS) {
        if (IS_WHITESPACE(input[i])) {
            if (word_len > 0) {
                current_word[word_len] = '\0';
                
                for (int j = 0; j < word_len; j++) {
                    current_word[j] = TO_LOWER(current_word[j]);
                }
                
                int found_idx = find_word(words, count, current_word);
                if (found_idx >= 0) {
                    if (words[found_idx].count < INT_MAX) {
                        words[found_idx].count++;
                    }
                } else {
                    if (safe_strncpy(words[count].word, current_word, MAX_WORD_LEN) == 0) {
                        words[count].count = 1;
                        count++;
                    }
                }
                word_len = 0;
            }
        } else if (isalpha((unsigned char)input[i])) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len] = input[i];
                word_len++;
            }
        }
        i++;
    }
    
    if (word_len > 0) {
        current_word[word_len] = '\0';
        for (int j = 0; j < word_len; j++) {
            current_word[j] = TO_LOWER(current_word[j]);
        }
        
        int found_idx = find_word(words, count, current_word);
        if (found_idx >= 0) {
            if (words[found_idx].count < INT_MAX) {
                words[found_idx].count++;
            }
        } else if (count < MAX_WORDS) {
            if (safe_strncpy(words[count].word, current_word, MAX_WORD_LEN) == 0) {
                words[count].count = 1;
                count++;
            }
        }
    }
    
    *word_count = count;
    return 0;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count > wb->count) return -1;
    if (wa->count < wb->count) return 1;
    return strcmp(wa->word, wb->word);
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_string(input)) {
        fprintf(stderr, "Invalid input string\n");
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequency count:\n");
        for (int i = 0