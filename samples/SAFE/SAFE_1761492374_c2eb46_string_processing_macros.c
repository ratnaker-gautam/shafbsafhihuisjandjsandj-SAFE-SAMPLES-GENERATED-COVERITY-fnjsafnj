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
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

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

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_input(const char *input, WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return;
    
    int i = 0;
    while (buffer[i] != '\0' && i < MAX_INPUT_LEN - 1) {
        while (IS_WHITESPACE(buffer[i]) && buffer[i] != '\0') i++;
        
        if (buffer[i] == '\0') break;
        
        char current_word[MAX_WORD_LEN];
        int j = 0;
        while (!IS_WHITESPACE(buffer[i]) && buffer[i] != '\0' && j < MAX_WORD_LEN - 1) {
            current_word[j] = TO_LOWER(buffer[i]);
            j++;
            i++;
        }
        current_word[j] = '\0';
        
        if (j > 0) {
            int index = find_word_index(words, *word_count, current_word);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else if (*word_count < MAX_WORDS) {
                if (safe_strncpy(words[*word_count].word, current_word, MAX_WORD_LEN) == 0) {
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
    }
}

void print_word_counts(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].word[0] = '\0';
        words[i].count = 0;
    }
    
    process_input(input, words, &word_count);
    
    if (word_count > 0) {
        printf("Word frequencies:\n");
        print_word_counts(words, word_count);
    } else {
        printf("No words found in input\n");
    }
    
    return 0;
}