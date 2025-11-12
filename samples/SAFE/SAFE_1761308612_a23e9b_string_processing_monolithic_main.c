//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64

int main(void) {
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
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = (int)input_len;
    
    int in_word = 0;
    int word_start = 0;
    int current_word_len = 0;
    
    for (int i = 0; i <= (int)input_len; i++) {
        if (i == (int)input_len || isspace((unsigned char)input[i])) {
            if (in_word) {
                if (current_word_len > 0 && current_word_len <= MAX_WORD_LEN && word_count < MAX_WORDS) {
                    strncpy(words[word_count], input + word_start, current_word_len);
                    words[word_count][current_word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
                current_word_len = 0;
            }
            if (i < (int)input_len && input[i] == '\n') {
                line_count++;
            }
        } else {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
            current_word_len++;
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
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        char longest_word[MAX_WORD_LEN + 1] = "";
        char shortest_word[MAX_WORD_LEN + 1] = "";
        int longest_len = 0;
        int shortest_len = MAX_WORD_LEN + 1;
        
        for (int i = 0; i < word_count; i++) {
            int len = (int)strlen(words[i]);
            if (len > longest_len) {
                longest_len = len;
                strncpy(longest_word, words[i], MAX_WORD_LEN);
                longest_word[MAX_WORD_LEN] = '\0';
            }
            if (len < shortest_len) {
                shortest_len = len;
                strncpy(shortest_word, words[i], MAX_WORD_LEN);
                shortest_word[MAX_WORD_LEN] = '\0';
            }
        }
        
        printf("\nLongest word: %s (%d characters)\n", longest_word, longest_len);
        printf("Shortest word: %s (%d characters)\n", shortest_word, shortest_len);
    }
    
    return 0;
}