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

int process_input(const char* input, WordCount* words, int* word_count) {
    if (!input || !words || !word_count) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN) return 0;
    strcpy(buffer, input);
    
    char* token = strtok(buffer, " \t\n\r.,;:!?\"()[]{}");
    int count = 0;
    
    while (token && count < MAX_WORDS) {
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
                for (size_t i = 0; i < len; i++) {
                    words[count].word[i] = tolower((unsigned char)token[i]);
                }
                words[count].word[len] = '\0';
                words[count].count = 1;
                count++;
            }
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"()[]{}");
    }
    
    *word_count = count;
    return 1;
}

void consolidate_words(WordCount* words, int* word_count) {
    if (!words || !word_count || *word_count <= 0) return;
    
    for (int i = 0; i < *word_count; i++) {
        for (int j = i + 1; j < *word_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count += words[j].count;
                for (int k = j; k < *word_count - 1; k++) {
                    strcpy(words[k].word, words[k + 1].word);
                    words[k].count = words[k + 1].count;
                }
                (*word_count)--;
                j--;
            }
        }
    }
}

int compare_word_count(const void* a, const void* b) {
    const WordCount* wa = (const WordCount*)a;
    const WordCount* wb = (const WordCount*)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main() {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d chars):\n", MAX_INPUT_LEN - 1);
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    consolidate_words(words, &word_count);
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}