//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))

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

int is_valid_char(char c) {
    return (c >= 32 && c <= 126);
}

int split_into_words(const char *input, WordCount words[], int max_words) {
    int word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t i = 0;
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        size_t word_start = i;
        size_t word_len = 0;
        
        while (i < input_len && !IS_WHITESPACE(input[i]) && word_len < MAX_WORD_LEN - 1) {
            if (is_valid_char(input[i])) {
                word_len++;
            }
            i++;
        }
        
        if (word_len > 0) {
            size_t copy_len = (word_len < MAX_WORD_LEN - 1) ? word_len : MAX_WORD_LEN - 1;
            for (size_t j = 0; j < copy_len; j++) {
                words[word_count].word[j] = TO_LOWER(input[word_start + j]);
            }
            words[word_count].word[copy_len] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void count_word_frequencies(WordCount words[], int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (words[i].count == 0) {
            continue;
        }
        
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count == 0) {
                continue;
            }
            
            int match = 1;
            for (int k = 0; words[i].word[k] != '\0' && words[j].word[k] != '\0'; k++) {
                if (words[i].word[k] != words[j].word[k]) {
                    match = 0;
                    break;
                }
            }
            
            if (match && words[i].word[0] != '\0' && words[j].word[0] != '\0') {
                words[i].count += words[j].count;
                words[j].count = 0;
            }
        }
    }
}

void print_word_frequencies(const WordCount words[], int word_count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0 && words[i].word[0] != '\0') {
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
    
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1 && input[MAX_INPUT_LEN - 1] != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Input too long, truncating to %d characters\n", MAX_INPUT_LEN);
    }
    
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].word[0] = '\0';
        words[i].count = 0;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    count_word_frequencies(words, word_count);
    print_word_frequencies(words, word_count);
    
    return 0;
}