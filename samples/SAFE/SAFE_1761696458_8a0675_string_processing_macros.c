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
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

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

int safe_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -1;
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void normalize_word(char *word) {
    if (word == NULL) return;
    char *read = word;
    char *write = word;
    while (*read) {
        if (IS_ALPHA(*read)) {
            *write = TO_LOWER(*read);
            write++;
        }
        read++;
    }
    *write = '\0';
}

int tokenize_input(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return 0;
    int word_count = 0;
    const char *p = input;
    
    while (*p && word_count < max_words) {
        while (*p && IS_WHITESPACE(*p)) p++;
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p && !IS_WHITESPACE(*p)) p++;
        
        size_t len = p - start;
        if (len >= MAX_WORD_LEN) len = MAX_WORD_LEN - 1;
        
        if (safe_strncpy(words[word_count], start, len + 1) == 0) {
            normalize_word(words[word_count]);
            if (words[word_count][0] != '\0') {
                word_count++;
            }
        }
    }
    return word_count;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, WordCount *word_counts) {
    if (words == NULL || word_counts == NULL || word_count <= 0) return 0;
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (safe_strcmp(words[i], word_counts[j].word) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && unique_count < MAX_WORDS) {
            if (safe_strncpy(word_counts[unique_count].word, words[i], MAX_WORD_LEN) == 0) {
                word_counts[unique_count].count = 1;
                unique_count++;
            }
        }
    }
    return unique_count;
}

void print_word_counts(WordCount *word_counts, int count) {
    if (word_counts == NULL || count <= 0) return;
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount word_counts[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (input[0] == '\0') {
        printf("No input provided.\n");
        return 1;
    }
    
    int word_count = tokenize_input(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, word_counts);
    if (unique_count == 0) {
        printf("Error processing words.\n");
        return 1;
    }
    
    print_word_counts(word_counts, unique_count);
    
    return 0;
}