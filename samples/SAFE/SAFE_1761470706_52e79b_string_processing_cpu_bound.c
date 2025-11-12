//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 100000
#define MAX_WORDS 50000
#define MAX_WORD_LEN 100

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void safe_strncpy(char *dest, const char *src, size_t n) {
    if (n == 0) return;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    const char *ptr = input;
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        while (*ptr != '\0' && !is_word_char(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') break;
        
        const char *word_start = ptr;
        while (*ptr != '\0' && is_word_char(*ptr)) {
            ptr++;
        }
        
        size_t word_len = ptr - word_start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char current_word[MAX_WORD_LEN];
        safe_strncpy(current_word, word_start, word_len + 1);
        
        for (size_t i = 0; i < word_len; i++) {
            current_word[i] = tolower((unsigned char)current_word[i]);
        }
        
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, current_word) == 0) {
                if (words[i].count < INT_MAX) {
                    words[i].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found) {
            safe_strncpy(words[word_count].word, current_word, sizeof(words[word_count].word));
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_counts);
    
    int output_count = word_count < 10 ? word_count : 10;
    for (int i = 0; i < output_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}