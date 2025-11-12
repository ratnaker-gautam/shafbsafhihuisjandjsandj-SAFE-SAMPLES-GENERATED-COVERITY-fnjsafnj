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
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int pos = 0;
    int word_start = -1;
    int word_position = 0;
    
    while (pos < (int)input_len) {
        if (is_word_char(input[pos])) {
            if (word_start == -1) {
                word_start = pos;
            }
        } else {
            if (word_start != -1) {
                int word_len = pos - word_start;
                if (word_len < MAX_WORD_LEN) {
                    char current_word[MAX_WORD_LEN];
                    strncpy(current_word, input + word_start, word_len);
                    current_word[word_len] = '\0';
                    
                    for (int i = 0; i < word_len; i++) {
                        current_word[i] = tolower((unsigned char)current_word[i]);
                    }
                    
                    find_or_add_word(words, &word_count, current_word, word_position);
                    word_position++;
                }
                word_start = -1;
            }
        }
        pos++;
    }
    
    if (word_start != -1) {
        int word_len = (int)input_len - word_start;
        if (word_len < MAX_WORD_LEN) {
            char current_word[MAX_WORD_LEN];
            strncpy(current_word, input + word_start, word_len);
            current_word[word_len] = '\0';
            
            for (int i = 0; i < word_len; i++) {
                current_word[i] = tolower((unsigned char)current_word[i]);
            }
            
            find_or_add_word(words, &word_count, current_word, word_position);
        }
    }
    
    if (word_count == 0) {
        fprintf(stderr, "No words found\n");
        return 1;
    }
    
    bubble_sort_words(words, word_count);
    
    int output_count = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < output_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}