//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

void to_lower_case(char *str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char *text, struct WordCount **result, int *unique_count) {
    char buffer[1024];
    if (strlen(text) >= sizeof(buffer)) {
        *result = NULL;
        *unique_count = 0;
        return;
    }
    
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    struct WordCount *words = malloc(MAX_WORDS * sizeof(struct WordCount));
    if (words == NULL) {
        *result = NULL;
        *unique_count = 0;
        return;
    }
    
    int word_count = 0;
    char *token = strtok(buffer, " \t\n\r.,;:!?\"()[]{}");
    
    while (token != NULL && word_count < MAX_WORDS) {
        size_t len = strlen(token);
        if (len > 0 && len < MAX_WORD_LEN) {
            int valid = 1;
            for (size_t i = 0; i < len; i++) {
                if (!is_valid_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            
            if (valid) {
                to_lower_case(token);
                int index = find_word_index(words, word_count, token);
                if (index >= 0) {
                    words[index].count++;
                } else {
                    strncpy(words[word_count].word, token, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"()[]{}");
    }
    
    *result = words;
    *unique_count = word_count;
}

void print_word_counts(struct WordCount *words, int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[2048];
    printf("Enter text to analyze (max %zu characters):\n", sizeof(input) - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    struct WordCount *words = NULL;
    int unique_count = 0;
    
    process_text(input, &words, &unique_count);
    
    if (words == NULL) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }
    
    if (unique_count > 0) {
        print_word_counts(words, unique_count);
    } else {
        printf("No valid words found in input\n");
    }
    
    free(words);
    return 0;
}