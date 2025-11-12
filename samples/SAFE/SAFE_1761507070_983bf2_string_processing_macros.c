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

int extract_words(const char *input, WordCount words[], int max_words) {
    int word_count = 0;
    int i = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) break;
        
        int start = i;
        while (i < input_len && !IS_WHITESPACE(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
            int j;
            for (j = 0; j < word_len; j++) {
                words[word_count].word[j] = TO_LOWER(input[start + j]);
            }
            words[word_count].word[word_len] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void count_word_frequency(WordCount words[], int word_count) {
    int i, j;
    for (i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            for (j = i + 1; j < word_count; j++) {
                if (words[j].count > 0 && strcmp(words[i].word, words[j].word) == 0) {
                    words[i].count++;
                    words[j].count = 0;
                }
            }
        }
    }
}

void print_word_frequency(WordCount words[], int word_count) {
    int i;
    printf("Word frequency analysis:\n");
    for (i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("'%s': %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    count_word_frequency(words, word_count);
    print_word_frequency(words, word_count);
    
    return 0;
}