//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
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

int safe_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -2;
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
        if (isalpha((unsigned char)*read)) {
            *write = TO_LOWER(*read);
            write++;
        }
        read++;
    }
    *write = '\0';
}

int split_into_words(const char *input, WordCount words[], int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return 0;
    int word_count = 0;
    const char *p = input;
    
    while (*p && word_count < max_words) {
        while (*p && IS_WHITESPACE(*p)) p++;
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p && !IS_WHITESPACE(*p)) p++;
        
        size_t word_len = p - start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        char temp_word[MAX_WORD_LEN];
        if (safe_strncpy(temp_word, start, word_len + 1) != 0) continue;
        
        normalize_word(temp_word);
        if (temp_word[0] == '\0') continue;
        
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (safe_strcmp(words[i].word, temp_word) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (safe_strncpy(words[word_count].word, temp_word, MAX_WORD_LEN) == 0) {
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    return word_count;
}

void sort_words(WordCount words[], int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (safe_strcmp(words[j].word, words[j + 1].word) > 0) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (input[0] == '\0') {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}