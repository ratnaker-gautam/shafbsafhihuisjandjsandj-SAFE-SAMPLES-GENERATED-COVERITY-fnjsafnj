//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *pos = buffer;
    while (*pos && word_count < MAX_WORDS) {
        while (*pos && !is_word_char(*pos)) {
            pos++;
        }
        
        if (!*pos) break;
        
        char *word_start = pos;
        while (*pos && is_word_char(*pos)) {
            pos++;
        }
        
        size_t word_len = pos - word_start;
        if (word_len >= sizeof(words[0].word)) {
            word_len = sizeof(words[0].word) - 1;
        }
        
        char current_word[sizeof(words[0].word)];
        strncpy(current_word, word_start, word_len);
        current_word[word_len] = '\0';
        to_lower_case(current_word);
        
        int found_index = find_word(words, word_count, current_word);
        if (found_index >= 0) {
            words[found_index].count++;
        } else {
            if (word_count < MAX_WORDS) {
                strncpy(words[word_count].word, current_word, sizeof(words[word_count].word) - 1);
                words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
        
        if (*pos) pos++;
    }
    
    printf("\nWord frequency analysis:\n");
    printf("Word\t\tCount\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-15s\t%d\n", words[i].word, words[i].count);
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
    }
    
    return 0;
}