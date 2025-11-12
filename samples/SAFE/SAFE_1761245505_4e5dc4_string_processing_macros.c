//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN + 1];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
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

int add_word(WordCount *words, int *count, const char *word) {
    if (*count >= MAX_WORDS) return 0;
    
    char normalized[MAX_WORD_LEN + 1];
    normalize_word(normalized, word, sizeof(normalized));
    
    if (safe_strlen(normalized, MAX_WORD_LEN) == 0) return 0;
    
    int index = find_word_index(words, *count, normalized);
    if (index >= 0) {
        words[index].count++;
        return 1;
    }
    
    strncpy(words[*count].word, normalized, MAX_WORD_LEN);
    words[*count].word[MAX_WORD_LEN] = '\0';
    words[*count].count = 1;
    (*count)++;
    return 1;
}

void extract_words(const char *input, WordCount *words, int *word_count) {
    *word_count = 0;
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    char *token = buffer;
    while (*token != '\0') {
        while (*token != '\0' && IS_WHITESPACE(*token)) {
            token++;
        }
        
        if (*token == '\0') break;
        
        char *word_start = token;
        while (*token != '\0' && !IS_WHITESPACE(*token)) {
            token++;
        }
        
        char temp_char = *token;
        *token = '\0';
        
        if (!add_word(words, word_count, word_start)) {
            break;
        }
        
        *token = temp_char;
        if (*token != '\0') token++;
    }
}

void print_word_counts(const WordCount *words, int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    extract_words(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}