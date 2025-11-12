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
    int i = 0;
    int in_word = 0;
    int current_word_len = 0;
    char current_word[MAX_WORD_LEN] = {0};
    
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    for (size_t pos = 0; pos < input_len && i < MAX_WORDS; pos++) {
        char c = input[pos];
        
        if (!is_valid_char(c)) {
            return 0;
        }
        
        if (IS_WHITESPACE(c)) {
            if (in_word && current_word_len > 0) {
                current_word[current_word_len] = '\0';
                
                int found = 0;
                for (int j = 0; j < *word_count; j++) {
                    if (strcmp(words[j].word, current_word) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found && *word_count < MAX_WORDS) {
                    strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
                
                current_word_len = 0;
                in_word = 0;
            }
        } else {
            if (current_word_len < MAX_WORD_LEN - 1) {
                current_word[current_word_len++] = TO_LOWER(c);
                in_word = 1;
            } else {
                return 0;
            }
        }
    }
    
    if (in_word && current_word_len > 0) {
        current_word[current_word_len] = '\0';
        
        int found = 0;
        for (int j = 0; j < *word_count; j++) {
            if (strcmp(words[j].word, current_word) == 0) {
                words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && *word_count < MAX_WORDS) {
            strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
            words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[*word_count].count = 1;
            (*word_count)++;
        }
    }
    
    return 1;
}

void sort_words(WordCount *words, int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    WordCount words[MAX_WORDS] = {0};
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (safe_strlen(input, MAX_INPUT_LEN) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!parse_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("Word\t\tCount\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < word_count; i++) {
        printf