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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, MAX_INPUT_LEN + 1);
    return len > 0 && len <= MAX_INPUT_LEN;
}

int split_into_words(const char *input, WordCount words[], int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) return 0;
    
    int count = 0;
    const char *p = input;
    
    while (*p != '\0' && count < MAX_WORDS) {
        while (IS_WHITESPACE(*p)) p++;
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p != '\0' && !IS_WHITESPACE(*p) && (p - start) < MAX_WORD_LEN - 1) {
            p++;
        }
        
        size_t word_len = p - start;
        if (word_len == 0) break;
        
        char temp_word[MAX_WORD_LEN];
        size_t i;
        for (i = 0; i < word_len && i < MAX_WORD_LEN - 1; i++) {
            temp_word[i] = TO_LOWER(start[i]);
        }
        temp_word[i] = '\0';
        
        int found = 0;
        for (int j = 0; j < count; j++) {
            if (strcmp(words[j].word, temp_word) == 0) {
                if (words[j].count < INT_MAX) {
                    words[j].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (safe_strlen(temp_word, MAX_WORD_LEN) >= MAX_WORD_LEN - 1) continue;
            strncpy(words[count].word, temp_word, MAX_WORD_LEN - 1);
            words[count].word[MAX_WORD_LEN - 1] = '\0';
            words[count].count = 1;
            count++;
        }
        
        while (IS_WHITESPACE(*p)) p++;
    }
    
    *word_count = count;
    return 1;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count > wa->count) return 1;
    if (wb->count < wa->count) return -1;
    return strcmp(wa->word, wb->word);
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
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!split_into_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("Word frequency count (sorted by frequency):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}