//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 4096
#define MAX_WORDS 1000
#define MAX_WORD_LEN 100

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int process_word(const char *word, WordCount *words, int *word_count) {
    if (word[0] == '\0') return 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            if (words[i].count < INT_MAX) {
                words[i].count++;
            }
            return 1;
        }
    }
    
    if (*word_count >= MAX_WORDS) return 0;
    
    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN) return 0;
    
    strcpy(words[*word_count].word, word);
    words[*word_count].count = 1;
    (*word_count)++;
    return 1;
}

void to_lowercase(char *str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void process_text(const char *text, WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(text) >= MAX_INPUT_LEN) return;
    
    strcpy(buffer, text);
    to_lowercase(buffer);
    
    char current_word[MAX_WORD_LEN];
    size_t word_pos = 0;
    
    for (size_t i = 0; buffer[i] != '\0'; i++) {
        if (is_valid_char(buffer[i])) {
            if (word_pos < MAX_WORD_LEN - 1) {
                current_word[word_pos++] = buffer[i];
            }
        } else {
            if (word_pos > 0) {
                current_word[word_pos] = '\0';
                process_word(current_word, words, word_count);
                word_pos = 0;
            }
        }
    }
    
    if (word_pos > 0) {
        current_word[word_pos] = '\0';
        process_word(current_word, words, word_count);
    }
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    process_text(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found.\n");
        return 1;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_counts);
    
    printf("\nWord frequency analysis (top 20):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    int display_count = word_count > 20 ? 20 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}