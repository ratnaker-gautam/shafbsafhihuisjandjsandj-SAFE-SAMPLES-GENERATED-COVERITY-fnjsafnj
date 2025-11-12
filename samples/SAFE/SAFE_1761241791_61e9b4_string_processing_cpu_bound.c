//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LENGTH 4096
#define MAX_WORD_COUNT 1024

typedef struct {
    char word[64];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

void to_lower_case(char *str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_text(const char *text, WordCount *words) {
    char buffer[MAX_INPUT_LENGTH];
    if (strlen(text) >= MAX_INPUT_LENGTH - 1) {
        return -1;
    }
    strncpy(buffer, text, MAX_INPUT_LENGTH - 1);
    buffer[MAX_INPUT_LENGTH - 1] = '\0';
    
    to_lower_case(buffer);
    
    int word_count = 0;
    char *token = buffer;
    char *end = buffer;
    
    while (*token != '\0' && word_count < MAX_WORD_COUNT) {
        while (*token != '\0' && !is_valid_char(*token)) {
            token++;
        }
        if (*token == '\0') {
            break;
        }
        
        end = token;
        while (*end != '\0' && is_valid_char(*end)) {
            end++;
        }
        
        size_t word_len = end - token;
        if (word_len >= sizeof(words[0].word)) {
            word_len = sizeof(words[0].word) - 1;
        }
        
        char current_word[64];
        strncpy(current_word, token, word_len);
        current_word[word_len] = '\0';
        
        int existing_index = find_word_index(words, word_count, current_word);
        if (existing_index != -1) {
            if (words[existing_index].count < INT_MAX) {
                words[existing_index].count++;
            }
        } else {
            strncpy(words[word_count].word, current_word, sizeof(words[word_count].word) - 1);
            words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
        
        token = end;
    }
    
    return word_count;
}

int main(void) {
    char input_text[MAX_INPUT_LENGTH];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LENGTH - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input_text[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    WordCount words[MAX_WORD_COUNT];
    for (int i = 0; i < MAX_WORD_COUNT; i++) {
        words[i].word[0] = '\0';
        words[i].count = 0;
    }
    
    int total_words = process_text(input_text, words);
    if (total_words < 0) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (total_words == 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    qsort(words, total_words, sizeof(WordCount), compare_word_counts);
    
    printf("\nWord frequency analysis (sorted by frequency):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < total_words; i++) {
        if (words[i].count > 0) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    }
    
    return 0;
}