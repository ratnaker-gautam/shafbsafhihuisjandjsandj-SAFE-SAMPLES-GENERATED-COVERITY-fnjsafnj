//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void process_text(const char *text, WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(text) >= MAX_INPUT_LEN - 1) {
        return;
    }
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    char *token = buffer;
    int pos = 0;
    
    while (*token && pos < MAX_INPUT_LEN - 1) {
        while (*token && !is_valid_char(*token)) {
            token++;
            pos++;
        }
        
        if (!*token) break;
        
        char current_word[MAX_WORD_LEN] = {0};
        int word_len = 0;
        
        while (*token && is_valid_char(*token) && word_len < MAX_WORD_LEN - 1) {
            current_word[word_len++] = *token;
            token++;
            pos++;
        }
        current_word[word_len] = '\0';
        
        if (word_len > 0) {
            to_lowercase(current_word);
            
            int existing_index = find_word(words, *word_count, current_word);
            if (existing_index != -1) {
                if (words[existing_index].count < INT_MAX) {
                    words[existing_index].count++;
                }
            } else if (*word_count < MAX_WORDS - 1) {
                strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
                words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN] = {0};
    WordCount words[MAX_WORDS] = {0};
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    process_text(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_words);
    
    printf("\nWord frequency analysis:\n");
    printf("Word\t\tCount\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-15s\t%d\n", words[i].word, words[i].count);
    }
    
    int total_words = 0;
    for (int i = 0; i < word_count; i++) {
        if (total_words <= INT_MAX - words[i].count) {
            total_words += words[i].count;
        } else {
            total_words = INT_MAX;
            break;
        }
    }
    
    printf("\nTotal unique words: %d\n", word_count);
    printf("Total word occurrences: %d\n", total_words);
    
    return 0;
}