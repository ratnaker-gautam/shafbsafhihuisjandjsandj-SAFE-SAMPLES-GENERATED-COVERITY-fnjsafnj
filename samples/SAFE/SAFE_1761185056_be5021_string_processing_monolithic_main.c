//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int line_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
        printf("Empty input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = input_len;
    
    int in_word = 0;
    int word_start = 0;
    int current_word_len = 0;
    
    for (int i = 0; i <= input_len; i++) {
        if (i == input_len || isspace(input[i])) {
            if (in_word) {
                if (current_word_len > 0 && current_word_len <= MAX_WORD_LEN && word_count < MAX_WORDS) {
                    strncpy(words[word_count], input + word_start, current_word_len);
                    words[word_count][current_word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
                current_word_len = 0;
            }
        } else {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
            current_word_len++;
        }
    }
    
    for (int i = 0; i <= input_len; i++) {
        if (input[i] == '\n') {
            line_count++;
        }
    }
    if (input_len > 0) {
        line_count++;
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    
    if (word_count > 0) {
        printf("\nWords found:\n");
        for (int i = 0; i < word_count; i++) {
            if (i < MAX_WORDS) {
                printf("%d: %s\n", i + 1, words[i]);
            }
        }
        
        int longest_index = 0;
        int longest_len = strlen(words[0]);
        for (int i = 1; i < word_count; i++) {
            int len = strlen(words[i]);
            if (len > longest_len) {
                longest_len = len;
                longest_index = i;
            }
        }
        
        printf("\nLongest word: %s (%d characters)\n", words[longest_index], longest_len);
    }
    
    return 0;
}