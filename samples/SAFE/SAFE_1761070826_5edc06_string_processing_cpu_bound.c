//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
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
    
    if (*word_count >= MAX_WORDS) {
        return -1;
    }
    
    strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
    words[*word_count].count = 1;
    words[*word_count].first_pos = position;
    (*word_count)++;
    return *word_count - 1;
}

void bubble_sort_words(WordInfo words[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && words[j].first_pos > words[j + 1].first_pos)) {
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
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int pos = 0;
    int word_start = -1;
    int word_position = 0;
    
    for (int i = 0; i <= len; i++) {
        if (i < len && is_word_char(input[i])) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    char current_word[MAX_WORD_LEN];
                    strncpy(current_word, &input[word_start], word_len);
                    current_word[word_len] = '\0';
                    
                    for (int j = 0; j < word_len; j++) {
                        current_word[j] = tolower((unsigned char)current_word[j]);
                    }
                    
                    if (find_or_add_word(words, &word_count, current_word, word_position) == -1) {
                        fprintf(stderr, "Too many unique words\n");
                        return 1;
                    }
                    word_position++;
                }
                word_start = -1;
            }
        }
        pos++;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    bubble_sort_words(words, word_count);
    
    printf("\nWord frequency analysis (sorted by frequency, then by first occurrence):\n");
    printf("%-20s %-10s %s\n", "Word", "Count", "First Position");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-20s %-10d %d\n", words[i].word, words[i].count, words[i].first_pos);
    }
    
    return 0;
}