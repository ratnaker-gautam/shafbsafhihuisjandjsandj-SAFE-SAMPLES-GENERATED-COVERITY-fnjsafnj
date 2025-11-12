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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = input_len;
    line_count = 1;
    
    int in_word = 0;
    int word_start = 0;
    int current_word_len = 0;
    
    for (int i = 0; i <= input_len; i++) {
        if (i < input_len && !isspace((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
                current_word_len = 0;
            }
            current_word_len++;
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS && current_word_len <= MAX_WORD_LEN) {
                    strncpy(words[word_count], &input[word_start], current_word_len);
                    words[word_count][current_word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    printf("\nText analysis:\n");
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    
    if (word_count > 0) {
        printf("\nWords found:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord lengths:\n");
        for (int i = 0; i < word_count; i++) {
            int len = strlen(words[i]);
            printf("%s: %d characters\n", words[i], len);
        }
        
        int total_word_chars = 0;
        for (int i = 0; i < word_count; i++) {
            total_word_chars += strlen(words[i]);
        }
        
        if (word_count > 0) {
            double avg_length = (double)total_word_chars / word_count;
            printf("\nAverage word length: %.2f\n", avg_length);
        }
        
        char longest_word[MAX_WORD_LEN + 1] = "";
        char shortest_word[MAX_WORD_LEN + 1] = "";
        int max_len = 0;
        int min_len = MAX_WORD_LEN;
        
        for (int i = 0; i < word_count; i++) {
            int len = strlen(words[i]);
            if (len > max_len) {
                max_len = len;
                strcpy(longest_word, words[i]);
            }
            if (len < min_len) {
                min_len = len;
                strcpy(shortest_word, words[i]);
            }
        }
        
        if (max_len > 0) {
            printf("Longest word: %s (%d characters)\n", longest_word, max_len);
        }
        if (min_len > 0) {
            printf("Shortest word: %s (%d characters)\n", shortest_word, min_len);
        }
    }
    
    return 0;
}