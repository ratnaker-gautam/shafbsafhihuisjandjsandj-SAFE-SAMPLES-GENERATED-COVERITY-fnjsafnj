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
    
    int in_word = 0;
    int word_start = 0;
    int current_word_len = 0;
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c)) {
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
    
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord List:\n");
        for (int i = 0; i < word_count; i++) {
            if (i < MAX_WORDS) {
                printf("%d: %s\n", i + 1, words[i]);
            }
        }
    }
    
    printf("\nWord Length Statistics:\n");
    int length_counts[11] = {0};
    
    for (int i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        if (len >= 0 && len <= 10) {
            length_counts[len]++;
        } else if (len > 10) {
            length_counts[10]++;
        }
    }
    
    for (int i = 0; i <= 10; i++) {
        if (i < 10) {
            printf("%d-letter words: %d\n", i, length_counts[i]);
        } else {
            printf("10+ letter words: %d\n", length_counts[10]);
        }
    }
    
    if (word_count > 0) {
        char longest_word[MAX_WORD_LEN + 1];
        char shortest_word[MAX_WORD_LEN + 1];
        int max_len = 0;
        int min_len = MAX_WORD_LEN;
        
        strcpy(longest_word, words[0]);
        strcpy(shortest_word, words[0]);
        max_len = strlen(words[0]);
        min_len = strlen(words[0]);
        
        for (int i = 1; i < word_count; i++) {
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
        
        printf("\nLongest word: %s (%d letters)\n", longest_word, max_len);
        printf("Shortest word: %s (%d letters)\n", shortest_word, min_len);
    }
    
    return 0;
}