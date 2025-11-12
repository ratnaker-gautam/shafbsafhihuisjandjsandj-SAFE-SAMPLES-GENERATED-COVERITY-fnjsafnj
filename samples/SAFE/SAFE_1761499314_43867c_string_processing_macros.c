//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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

void safe_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int is_valid_input(const char *str) {
    size_t len = safe_strlen(str, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i]) && !IS_WHITESPACE(str[i])) {
            return 0;
        }
    }
    return 1;
}

void normalize_word(char *word) {
    size_t len = safe_strlen(word, MAX_WORD_LEN + 1);
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        word[i] = TO_LOWER(word[i]);
    }
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int extract_words(const char *input, WordCount *words, int *word_count) {
    const char *p = input;
    int count = 0;
    
    while (*p != '\0' && count < MAX_WORDS) {
        while (IS_WHITESPACE(*p)) p++;
        
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p != '\0' && !IS_WHITESPACE(*p)) p++;
        
        size_t word_len = p - start;
        if (word_len > 0 && word_len <= MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN + 1];
            safe_strncpy(temp_word, start, word_len + 1);
            normalize_word(temp_word);
            
            int index = find_word_index(words, count, temp_word);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else {
                safe_strncpy(words[count].word, temp_word, MAX_WORD_LEN + 1);
                words[count].count = 1;
                count++;
            }
        }
    }
    
    *word_count = count;
    return 1;
}

void print_word_counts(WordCount *words, int count) {
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
        return EXIT_FAILURE;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return EXIT_FAILURE;
    }
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return EXIT_FAILURE;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
    } else {
        print_word_counts(words, word_count);
    }
    
    return EXIT_SUCCESS;
}