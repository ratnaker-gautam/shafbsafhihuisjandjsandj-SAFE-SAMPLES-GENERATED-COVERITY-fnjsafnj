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
    int first_position;
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
        words[*word_count].first_position = position;
        (*word_count)++;
        return *word_count - 1;
    }
    return -1;
}

void bubble_sort_words(WordInfo words[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
               (words[j].count == words[j + 1].count && 
                words[j].first_position > words[j + 1].first_position)) {
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
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        return 1;
    }
    
    int pos = 0;
    int word_start = -1;
    int word_position = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && is_word_char(input[i])) {
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
                    
                    find_or_add_word(words, &word_count, current_word, word_position);
                    word_position++;
                }
                word_start = -1;
            }
        }
    }
    
    bubble_sort_words(words, word_count);
    
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}