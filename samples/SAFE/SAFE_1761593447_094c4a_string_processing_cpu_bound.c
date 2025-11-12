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
    int first_pos;
} WordInfo;

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int find_or_add_word(WordInfo words[], int *word_count, const char *word, int position) {
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return i;
        }
    }
    
    if (*word_count < MAX_WORDS) {
        strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
        words[*word_count].count = 1;
        words[*word_count].first_pos = position;
        (*word_count)++;
        return *word_count - 1;
    }
    return -1;
}

void process_text(const char *text, WordInfo words[], int *word_count) {
    *word_count = 0;
    int len = strlen(text);
    int pos = 0;
    int word_start = -1;
    
    for (int i = 0; i <= len; i++) {
        if (i < len && is_word_char(text[i])) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    char temp_word[MAX_WORD_LEN];
                    strncpy(temp_word, text + word_start, word_len);
                    temp_word[word_len] = '\0';
                    
                    for (int j = 0; j < word_len; j++) {
                        temp_word[j] = tolower((unsigned char)temp_word[j]);
                    }
                    
                    find_or_add_word(words, word_count, temp_word, pos);
                    pos++;
                }
                word_start = -1;
            }
        }
    }
}

int compare_words(const void *a, const void *b) {
    const WordInfo *wa = (const WordInfo *)a;
    const WordInfo *wb = (const WordInfo *)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return wa->first_pos - wb->first_pos;
}

void sort_words(WordInfo words[], int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (compare_words(&words[j], &words[j + 1]) > 0) {
                WordInfo temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_text(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis (sorted by frequency, then by first occurrence):\n");
    printf("%-20s %-10s %-15s\n", "Word", "Count", "First Position");
    printf("-------------------------------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %-10d %-15d\n", words[i].word, words[i].count, words[i].first_pos);
    }
    
    int total_words = 0;
    for (int i = 0; i < word_count; i++) {
        total_words += words[i].count;
    }
    printf("\nTotal unique words: %d\n", word_count);
    printf("Total word occurrences: %d\n", total_words);
    
    return 0;
}