//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char text[MAX_WORD_LEN];
    size_t length;
} Word;

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    Word words[MAX_WORDS] = {0};
    size_t word_count = 0;
    size_t current_word_len = 0;
    
    printf("Enter a sentence (max %d characters): ", MAX_INPUT_LEN);
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
    
    for (size_t i = 0; i <= input_len && word_count < MAX_WORDS; i++) {
        char c = input[i];
        
        if (IS_WHITESPACE(c) || c == '\0' || IS_TERMINATOR(c)) {
            if (current_word_len > 0) {
                if (current_word_len >= MAX_WORD_LEN) {
                    fprintf(stderr, "Word too long\n");
                    return 1;
                }
                words[word_count].length = current_word_len;
                word_count++;
                current_word_len = 0;
            }
        } else if (isalpha((unsigned char)c)) {
            if (current_word_len < MAX_WORD_LEN - 1) {
                words[word_count].text[current_word_len] = tolower((unsigned char)c);
                current_word_len++;
            } else {
                fprintf(stderr, "Word too long\n");
                return 1;
            }
        } else {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("Processed words (%zu total):\n", word_count);
    for (size_t i = 0; i < word_count; i++) {
        words[i].text[words[i].length] = '\0';
        printf("%s", words[i].text);
        if (i < word_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    size_t total_chars = 0;
    for (size_t i = 0; i < word_count; i++) {
        total_chars += words[i].length;
    }
    
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    
    size_t max_len = 0;
    size_t min_len = MAX_WORD_LEN;
    for (size_t i = 0; i < word_count; i++) {
        if (words[i].length > max_len) max_len = words[i].length;
        if (words[i].length < min_len) min_len = words[i].length;
    }
    
    printf("Longest word length: %zu\n", max_len);
    printf("Shortest word length: %zu\n", min_len);
    
    return 0;
}