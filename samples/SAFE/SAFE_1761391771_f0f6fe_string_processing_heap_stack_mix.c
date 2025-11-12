//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char **split_into_words(const char *input, int *word_count) {
    if (!input || !word_count) return NULL;
    
    char **words = malloc(MAX_WORDS * sizeof(char *));
    if (!words) return NULL;
    
    *word_count = 0;
    int len = strlen(input);
    int start = -1;
    
    for (int i = 0; i <= len && *word_count < MAX_WORDS; i++) {
        if (i < len && is_valid_char(input[i])) {
            if (start == -1) start = i;
        } else if (start != -1) {
            int word_len = i - start;
            if (word_len > 0 && word_len <= MAX_WORD_LEN) {
                words[*word_count] = malloc(word_len + 1);
                if (words[*word_count]) {
                    strncpy(words[*word_count], input + start, word_len);
                    words[*word_count][word_len] = '\0';
                    (*word_count)++;
                }
            }
            start = -1;
        }
    }
    
    return words;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wc1 = (const WordCount *)a;
    const WordCount *wc2 = (const WordCount *)b;
    return strcmp(wc1->word, wc2->word);
}

WordCount *count_words(char **words, int word_count, int *unique_count) {
    if (!words || !unique_count || word_count <= 0) return NULL;
    
    WordCount *counts = malloc(word_count * sizeof(WordCount));
    if (!counts) return NULL;
    
    *unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (!words[i]) continue;
        
        int found = 0;
        for (int j = 0; j < *unique_count; j++) {
            if (strcmp(words[i], counts[j].word) == 0) {
                counts[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            counts[*unique_count].word = malloc(strlen(words[i]) + 1);
            if (counts[*unique_count].word) {
                strcpy(counts[*unique_count].word, words[i]);
                counts[*unique_count].count = 1;
                (*unique_count)++;
            }
        }
    }
    
    return counts;
}

void process_input(const char *input) {
    if (!input || strlen(input) == 0) {
        printf("No input provided.\n");
        return;
    }
    
    int word_count = 0;
    char **words = split_into_words(input, &word_count);
    if (!words || word_count == 0) {
        printf("No valid words found.\n");
        return;
    }
    
    int unique_count = 0;
    WordCount *word_counts = count_words(words, word_count, &unique_count);
    
    if (word_counts && unique_count > 0) {
        qsort(word_counts, unique_count, sizeof(WordCount), compare_words);
        
        printf("Word frequencies:\n");
        for (int i = 0; i < unique_count; i++) {
            printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        if (words[i]) free(words[i]);
    }
    free(words);
    
    if (word_counts) {
        for (int i = 0; i < unique_count; i++) {
            if (word_counts[i].word) free(word_counts[i].word);
        }
        free(word_counts);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin)) {
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) > MAX_INPUT_LEN) {
            printf("Input too long. Truncating to %d characters.\n", MAX_INPUT_LEN);
            input[MAX_INPUT_LEN] = '\0';
        }
        
        process_input(input);
    } else {
        printf("Error reading input.\n");
    }
    
    return 0;
}