//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 512

struct WordCount {
    char word[64];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int safe_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -1;
    return strcmp(s1, s2);
}

void safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return;
    strncpy(dest, src, n - 1);
    dest[n - 1] = '\0';
}

int process_input(const char *input, struct WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    safe_strncpy(buffer, input, sizeof(buffer));
    
    int len = strlen(buffer);
    if (len == 0) return 0;
    
    int current_word_start = -1;
    int current_word_len = 0;
    char current_word[64];
    
    for (int i = 0; i <= len; i++) {
        char c = buffer[i];
        
        if (i < len && is_valid_char(c)) {
            if (current_word_start == -1) {
                current_word_start = i;
                current_word_len = 0;
            }
            if (current_word_len < 63) {
                current_word[current_word_len++] = tolower((unsigned char)c);
            }
        } else {
            if (current_word_start != -1 && current_word_len > 0) {
                current_word[current_word_len] = '\0';
                
                int found = -1;
                for (int j = 0; j < *word_count; j++) {
                    if (safe_strcmp(words[j].word, current_word) == 0) {
                        found = j;
                        break;
                    }
                }
                
                if (found >= 0) {
                    if (words[found].count < 1000000) {
                        words[found].count++;
                    }
                } else if (*word_count < MAX_WORD_COUNT) {
                    safe_strncpy(words[*word_count].word, current_word, sizeof(words[*word_count].word));
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
                
                current_word_start = -1;
                current_word_len = 0;
            }
        }
    }
    
    return 0;
}

void print_results(const struct WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("=======================\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].word[0] != '\0') {
            printf("%-20s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORD_COUNT];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        printf("Error processing input.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    print_results(words, word_count);
    
    return 0;
}