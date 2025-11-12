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
    return isalnum((unsigned char)c) || c == '_';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) {
        return -1;
    }
    
    char buffer[MAX_INPUT_LEN + 1];
    size_t input_len = strlen(input);
    if (input_len > MAX_INPUT_LEN) {
        return -1;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    *word_count = 0;
    int in_word = 0;
    size_t word_start = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && is_valid_char(buffer[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN && *word_count < MAX_WORDS) {
                    char current_word[MAX_WORD_LEN];
                    strncpy(current_word, buffer + word_start, word_len);
                    current_word[word_len] = '\0';
                    
                    for (int j = 0; j < word_len; j++) {
                        current_word[j] = tolower((unsigned char)current_word[j]);
                    }
                    
                    int found = -1;
                    for (int j = 0; j < *word_count; j++) {
                        if (strcmp(words[j].word, current_word) == 0) {
                            found = j;
                            break;
                        }
                    }
                    
                    if (found >= 0) {
                        if (words[found].count < INT_MAX) {
                            words[found].count++;
                        }
                    } else {
                        strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
                        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                in_word = 0;
            }
        }
    }
    
    return 0;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequency analysis:\n");
        printf("%-20s %s\n", "Word", "Count");
        printf("%-20s %s\n", "----", "-----");
        
        for (int i = 0; i < word_count; i++) {
            if (words[i].count > 0) {
                printf("%-20s %d\n", words[i].word, words[i].count);
            }
        }
        
        int total_words = 0;
        for (int i = 0; i < word_count; i++) {
            if (total_words <= INT_MAX - words[i].count) {
                total_words += words[i].count;
            }
        }
        printf("\nTotal unique words: %d\n", word_count);
        printf("Total word occurrences: %d\n", total_words);
    } else {
        printf("No valid words found in input\n");
    }
    
    return 0;
}