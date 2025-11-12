//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

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

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, max_len);
    return len > 0 && len < max_len;
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    size_t j = 0;
    while (src[i] != '\0' && j < max_len - 1) {
        char c = TO_LOWER(src[i]);
        if (isalpha(c)) {
            dest[j] = c;
            j++;
        }
        i++;
    }
    dest[j] = '\0';
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!is_valid_input(input, sizeof(input))) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    size_t pos = 0;
    
    while (pos < input_len && word_count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        size_t start = pos;
        while (pos < input_len && !IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        size_t word_len = pos - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char raw_word[MAX_WORD_LEN];
            char normalized_word[MAX_WORD_LEN];
            
            if (word_len >= sizeof(raw_word)) {
                continue;
            }
            
            memcpy(raw_word, &input[start], word_len);
            raw_word[word_len] = '\0';
            
            normalize_word(normalized_word, raw_word, sizeof(normalized_word));
            
            if (normalized_word[0] != '\0') {
                int index = find_word_index(words, word_count, normalized_word);
                if (index >= 0) {
                    if (words[index].count < INT_MAX) {
                        words[index].count++;
                    }
                } else {
                    if (word_count < MAX_WORDS) {
                        strncpy(words[word_count].word, normalized_word, sizeof(words[word_count].word) - 1);
                        words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }
    }
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}