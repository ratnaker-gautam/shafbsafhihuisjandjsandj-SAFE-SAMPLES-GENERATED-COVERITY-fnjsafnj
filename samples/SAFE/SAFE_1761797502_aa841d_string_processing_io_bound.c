//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int find_word(struct WordCount *words, int count, const char *word) {
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

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
        return 0;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = c;
            in_word = 1;
        } else {
            if (in_word) {
                buffer[buf_pos] = '\0';
                to_lower_case(buffer);
                
                if (buf_pos > 0) {
                    int found_idx = find_word(words, word_count, buffer);
                    if (found_idx >= 0) {
                        words[found_idx].count++;
                    } else if (word_count < MAX_WORDS) {
                        strncpy(words[word_count].word, buffer, MAX_WORD_LEN - 1);
                        words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
                
                buf_pos = 0;
                in_word = 0;
            }
        }
    }
    
    printf("\nWord frequency analysis:\n");
    printf("========================\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
    }
    
    return 0;
}