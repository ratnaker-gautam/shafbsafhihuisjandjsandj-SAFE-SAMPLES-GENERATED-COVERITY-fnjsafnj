//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
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

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return i;
}

int normalize_word(char *word) {
    int len = safe_strlen(word, MAX_WORD_LEN);
    if (len <= 0) return 0;
    
    int i, j = 0;
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)word[i])) {
            word[j++] = TO_LOWER(word[i]);
        }
    }
    word[j] = '\0';
    return j;
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_input(const char *input, WordCount *words) {
    int word_count = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    int i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) break;
        
        int start = i;
        while (i < input_len && !IS_WHITESPACE(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            safe_strncpy(temp_word, &input[start], word_len + 1);
            temp_word[word_len] = '\0';
            
            if (normalize_word(temp_word) > 0) {
                int index = find_word_index(words, word_count, temp_word);
                if (index >= 0) {
                    words[index].count++;
                } else {
                    safe_strncpy(words[word_count].word, temp_word, MAX_WORD_LEN);
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }
    
    return word_count;
}

void sort_words(WordCount *words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(words[j].word, words[j + 1].word) > 0) {
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
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len >= MAX_INPUT_LEN - 1 && input[MAX_INPUT_LEN - 2] != '\n') {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int word_count = process_input(input, words);
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}