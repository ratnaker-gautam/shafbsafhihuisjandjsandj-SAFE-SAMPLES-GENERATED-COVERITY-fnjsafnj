//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

struct WordCount {
    char word[64];
    int count;
};

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int find_word(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void sort_words(struct WordCount *words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
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
        printf("No input provided\n");
        return 1;
    }
    
    int pos = 0;
    while (pos < (int)input_len && word_count < MAX_WORDS) {
        while (pos < (int)input_len && !is_word_char(input[pos])) {
            pos++;
        }
        
        if (pos >= (int)input_len) {
            break;
        }
        
        int start = pos;
        while (pos < (int)input_len && is_word_char(input[pos])) {
            pos++;
        }
        
        int word_len = pos - start;
        if (word_len >= 64) {
            word_len = 63;
        }
        
        char current_word[64];
        for (int i = 0; i < word_len; i++) {
            current_word[i] = tolower((unsigned char)input[start + i]);
        }
        current_word[word_len] = '\0';
        
        int found_index = find_word(words, word_count, current_word);
        if (found_index >= 0) {
            if (words[found_index].count < INT_MAX) {
                words[found_index].count++;
            }
        } else {
            if (word_count < MAX_WORDS) {
                strncpy(words[word_count].word, current_word, sizeof(words[word_count].word) - 1);
                words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis (top %d words):\n", word_count < 10 ? word_count : 10);
    int display_count = word_count < 10 ? word_count : 10;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}