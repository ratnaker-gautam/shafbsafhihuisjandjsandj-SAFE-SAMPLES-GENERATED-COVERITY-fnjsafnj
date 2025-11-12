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

int parse_words(const char *input, WordCount *words, int *word_count) {
    int len = safe_strlen(input, MAX_INPUT_LEN);
    if (len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    int count = 0;
    int pos = 0;
    
    while (pos < len && count < MAX_WORDS) {
        while (pos < len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        int word_start = pos;
        int word_len = 0;
        
        while (pos < len && !IS_WHITESPACE(input[pos]) && word_len < MAX_WORD_LEN - 1) {
            if (!is_valid_char(input[pos])) {
                return -1;
            }
            pos++;
            word_len++;
        }
        
        if (word_len > 0) {
            for (int i = 0; i < word_len; i++) {
                char c = TO_LOWER(input[word_start + i]);
                words[count].word[i] = c;
            }
            words[count].word[word_len] = '\0';
            words[count].count = 1;
            count++;
        }
    }
    
    *word_count = count;
    return 0;
}

int count_unique_words(WordCount *words, int word_count) {
    for (int i = 0; i < word_count; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                for (int k = j; k < word_count - 1; k++) {
                    words[k] = words[k + 1];
                }
                word_count--;
                j--;
            }
        }
    }
    return word_count;
}

void sort_words(WordCount *words, int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
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
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len <= 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (parse_words(input, words, &word_count) != 0) {
        fprintf(stderr, "Error parsing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    word_count = count_unique_words(words, word_count);
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}