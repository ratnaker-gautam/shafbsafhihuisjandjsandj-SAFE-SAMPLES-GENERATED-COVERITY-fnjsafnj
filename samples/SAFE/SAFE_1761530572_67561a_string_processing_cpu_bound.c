//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

struct WordCount {
    char word[MAX_WORD_LEN];
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
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
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
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char current_word[MAX_WORD_LEN];
        for (int i = 0; i < word_len; i++) {
            current_word[i] = tolower((unsigned char)input[start + i]);
        }
        current_word[word_len] = '\0';
        
        if (word_len > 0) {
            int found_index = find_word(words, word_count, current_word);
            if (found_index >= 0) {
                words[found_index].count++;
            } else {
                if (word_count < MAX_WORDS) {
                    strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}