//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
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
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '\'' || c == '-';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) {
        return 0;
    }
    
    int len = safe_strlen(input, MAX_INPUT_LEN);
    if (len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    int current_word_len = 0;
    char current_word[MAX_WORD_LEN] = {0};
    *word_count = 0;
    
    for (int i = 0; i <= len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && current_word_len < MAX_WORD_LEN - 1) {
            current_word[current_word_len++] = TO_LOWER(c);
        } else if (current_word_len > 0) {
            current_word[current_word_len] = '\0';
            
            int found = 0;
            for (int j = 0; j < *word_count; j++) {
                if (strcmp(words[j].word, current_word) == 0) {
                    if (words[j].count < INT_MAX) {
                        words[j].count++;
                    }
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
            current_word[0] = '\0';
        }
        
        if (*word_count >= MAX_WORDS) {
            break;
        }
    }
    
    return 1;
}

void print_word_counts(const WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i].word[0] != '\0') {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN] = {0};
    WordCount words[MAX_WORDS] = {0};
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strnlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}