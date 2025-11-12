//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN);
    
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
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = input_len;
    line_count = 1;
    
    for (int i = 0; i < input_len; i++) {
        if (input[i] == '\n') {
            line_count++;
        }
    }
    
    int in_word = 0;
    int word_start = 0;
    int current_word_len = 0;
    
    for (int i = 0; i <= input_len && word_count < MAX_WORDS; i++) {
        char c = (i < input_len) ? input[i] : ' ';
        
        if (isalpha((unsigned char)c)) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
                current_word_len = 0;
            }
            if (current_word_len < MAX_WORD_LEN) {
                current_word_len++;
            }
        } else {
            if (in_word) {
                if (current_word_len > 0 && current_word_len <= MAX_WORD_LEN) {
                    int copy_len = current_word_len;
                    if (copy_len > MAX_WORD_LEN) {
                        copy_len = MAX_WORD_LEN;
                    }
                    strncpy(words[word_count], &input[word_start], copy_len);
                    words[word_count][copy_len] = '\0';
                    word_count++;
                }
                in_word = 0;
                current_word_len = 0;
            }
        }
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWords found:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord lengths:\n");
        for (int i = 0; i < word_count; i++) {
            size_t len = strlen(words[i]);
            printf("%s: %zu characters\n", words[i], len);
        }
    }
    
    char longest_word[MAX_WORD_LEN + 1] = "";
    char shortest_word[MAX_WORD_LEN + 1] = "";
    size_t max_len = 0;
    size_t min_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
            strncpy(longest_word, words[i], MAX_WORD_LEN);
            longest_word[MAX_WORD_LEN] = '\0';
        }
        if (len < min_len && len > 0) {
            min_len = len;
            strncpy(shortest_word, words[i], MAX_WORD_LEN);
            shortest_word[MAX_WORD_LEN] = '\0';
        }
    }
    
    if (word_count > 0) {
        printf("\nLongest word: %s (%zu characters)\n", longest_word, max_len);
        printf("Shortest word: %s (%zu characters)\n", shortest_word, min_len);
    }
    
    return 0;
}