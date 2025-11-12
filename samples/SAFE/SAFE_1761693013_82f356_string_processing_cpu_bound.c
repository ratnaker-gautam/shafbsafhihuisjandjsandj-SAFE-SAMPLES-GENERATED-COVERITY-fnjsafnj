//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, WordCount *words) {
    int word_count = 0;
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    
    for (int i = 0; input[i] && word_count < MAX_WORDS; i++) {
        if (is_valid_char(input[i])) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = input[i];
            }
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                to_lower_case(buffer);
                
                int existing_idx = find_word(words, word_count, buffer);
                if (existing_idx >= 0) {
                    words[existing_idx].count++;
                } else {
                    if (strlen(buffer) > 0) {
                        strncpy(words[word_count].word, buffer, MAX_WORD_LEN - 1);
                        words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
                buf_pos = 0;
            }
        }
    }
    
    if (buf_pos > 0 && word_count < MAX_WORDS) {
        buffer[buf_pos] = '\0';
        to_lower_case(buffer);
        int existing_idx = find_word(words, word_count, buffer);
        if (existing_idx >= 0) {
            words[existing_idx].count++;
        } else {
            if (strlen(buffer) > 0) {
                strncpy(words[word_count].word, buffer, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    return word_count;
}

void sort_words(WordCount *words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    int word_count = process_input(input, words);
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}