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
    line_count = 1;
    
    int in_word = 0;
    int word_start = 0;
    int current_word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalpha((unsigned char)c)) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
                current_word_len = 0;
            }
            current_word_len++;
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS && current_word_len <= MAX_WORD_LEN) {
                    size_t copy_len = current_word_len;
                    if (copy_len > MAX_WORD_LEN) {
                        copy_len = MAX_WORD_LEN;
                    }
                    strncpy(words[word_count], &input[word_start], copy_len);
                    words[word_count][copy_len] = '\0';
                    word_count++;
                }
                in_word = 0;
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
        
        printf("\nWord statistics:\n");
        int total_word_len = 0;
        int shortest_len = MAX_WORD_LEN;
        int longest_len = 0;
        
        for (int i = 0; i < word_count; i++) {
            int len = strlen(words[i]);
            total_word_len += len;
            if (len < shortest_len) {
                shortest_len = len;
            }
            if (len > longest_len) {
                longest_len = len;
            }
        }
        
        double avg_len = (double)total_word_len / word_count;
        printf("Average word length: %.2f\n", avg_len);
        printf("Shortest word length: %d\n", shortest_len);
        printf("Longest word length: %d\n", longest_len);
    }
    
    printf("\nProcessed text (uppercase): ");
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        if (isalpha((unsigned char)c)) {
            putchar(toupper((unsigned char)c));
        } else {
            putchar(c);
        }
    }
    printf("\n");
    
    return 0;
}