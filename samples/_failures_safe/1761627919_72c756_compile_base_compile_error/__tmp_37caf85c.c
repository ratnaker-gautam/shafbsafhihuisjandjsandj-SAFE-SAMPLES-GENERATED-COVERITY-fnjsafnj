//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
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

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    return len > 0 && len <= MAX_INPUT_LEN;
}

void safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int extract_words(const char *input, WordCount words[], int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    const char *p = input;
    
    while (*p != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(*p)) p++;
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p != '\0' && !IS_WHITESPACE(*p)) p++;
        
        size_t word_len = p - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            safe_strncpy(words[word_count].word, start, word_len + 1);
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void normalize_word(char *word) {
    if (word == NULL) return;
    char *p = word;
    while (*p != '\0') {
        *p = TO_LOWER(*p);
        p++;
    }
}

int find_word_index(WordCount words[], int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    
    char normalized[MAX_WORD_LEN];
    safe_strncpy(normalized, word, sizeof(normalized));
    normalize_word(normalized);
    
    for (int i = 0; i < count; i++) {
        char temp[MAX_WORD_LEN];
        safe_strncpy(temp, words[i].word, sizeof(temp));
        normalize_word(temp);
        
        if (strcmp(temp, normalized) == 0) {
            return i;
        }
    }
    return -1;
}

void count_word_frequencies(WordCount words[], int *word_count) {
    if (words == NULL || word_count == NULL || *word_count <= 0) return;
    
    int unique_count = 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (words[i].count == 0) continue;
        
        int existing_index = find_word_index(words, unique_count, words[i].word);
        if (existing_index >= 0) {
            words[existing_index].count++;
            words[i].count = 0;
        } else {
            if (unique_count != i) {
                safe_strncpy(words[unique_count].word, words[i].word, MAX_WORD_LEN);
                words[unique_count].count = words[i].count;
                words[i].count = 0;
            }
            unique_count++;
        }
    }
    
    *word_count = unique_count;
}

void sort_words_by_frequency(WordCount words[], int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0