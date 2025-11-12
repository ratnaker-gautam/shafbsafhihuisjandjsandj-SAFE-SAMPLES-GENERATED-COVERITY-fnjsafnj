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
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return -1;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 0;
}

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i]) && !IS_WHITESPACE(str[i])) {
            return 0;
        }
    }
    return 1;
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_sentence(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    to_lower_case(buffer);
    
    char temp[MAX_WORD_LEN];
    int temp_len = 0;
    int count = 0;
    
    for (size_t i = 0; buffer[i] != '\0' && count < MAX_WORDS; i++) {
        if (IS_WHITESPACE(buffer[i]) || IS_TERMINATOR(buffer[i])) {
            if (temp_len > 0) {
                temp[temp_len] = '\0';
                
                int found = 0;
                for (int j = 0; j < count; j++) {
                    if (strcmp(words[j].word, temp) == 0) {
                        if (words[j].count < INT_MAX) {
                            words[j].count++;
                        }
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    if (safe_strncpy(words[count].word, temp, MAX_WORD_LEN) == 0) {
                        words[count].count = 1;
                        count++;
                    }
                }
                temp_len = 0;
            }
            
            if (IS_TERMINATOR(buffer[i])) {
                break;
            }
        } else if (isalpha((unsigned char)buffer[i]) && temp_len < MAX_WORD_LEN - 1) {
            temp[temp_len++] = buffer[i];
        }
    }
    
    *word_count = count;
    return 0;
}

void print_word_counts(const WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS] = {0};
    int word_count = 0;
    
    printf("Enter a sentence (end with punctuation): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int has_terminator = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (IS_TERMINATOR(input[i])) {
            has_terminator = 1;
            break;
        }
    }
    
    if (!has_terminator) {
        fprintf(stderr, "Sentence must end with punctuation\n");
        return 1;
    }
    
    if (process_sentence(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing sentence\n");
        return 1;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}