//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LENGTH 4096
#define MAX_WORDS 1000
#define MAX_WORD_LENGTH 100

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int process_word(char *word, WordCount *words, int *word_count) {
    if (strlen(word) == 0) return 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            if (words[i].count < INT_MAX) {
                words[i].count++;
            }
            return 1;
        }
    }
    
    if (*word_count < MAX_WORDS) {
        strncpy(words[*word_count].word, word, MAX_WORD_LENGTH - 1);
        words[*word_count].word[MAX_WORD_LENGTH - 1] = '\0';
        words[*word_count].count = 1;
        (*word_count)++;
        return 1;
    }
    
    return 0;
}

void to_lowercase(char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(const char *input, WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LENGTH];
    char current_word[MAX_WORD_LENGTH];
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LENGTH - 1) {
        return 0;
    }
    
    strncpy(buffer, input, MAX_INPUT_LENGTH - 1);
    buffer[MAX_INPUT_LENGTH - 1] = '\0';
    
    size_t buf_len = strlen(buffer);
    size_t word_pos = 0;
    
    for (size_t i = 0; i <= buf_len; i++) {
        if (i < buf_len && is_valid_char(buffer[i])) {
            if (word_pos < MAX_WORD_LENGTH - 1) {
                current_word[word_pos++] = buffer[i];
            }
        } else {
            if (word_pos > 0) {
                current_word[word_pos] = '\0';
                to_lowercase(current_word);
                if (!process_word(current_word, words, word_count)) {
                    return 0;
                }
                word_pos = 0;
            }
        }
    }
    
    return 1;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LENGTH];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency analysis (top %d words):\n", word_count > 10 ? 10 : word_count);
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    int display_count = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}