//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
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
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i <= input_len && word_count < MAX_WORDS; i++) {
        char c = input[i];
        
        if (IS_WHITESPACE(c) || IS_TERMINATOR(c) || c == '\0') {
            if (current_word_len > 0) {
                if (current_word_len >= MAX_WORD_LEN) {
                    printf("Word too long\n");
                    return EXIT_FAILURE;
                }
                words[word_count].length = current_word_len;
                word_count++;
                current_word_len = 0;
            }
            
            if (IS_TERMINATOR(c)) {
                break;
            }
        } else if (isalpha((unsigned char)c)) {
            if (current_word_len >= MAX_WORD_LEN - 1) {
                printf("Word too long\n");
                return EXIT_FAILURE;
            }
            words[word_count].text[current_word_len] = tolower((unsigned char)c);
            current_word_len++;
        } else {
            printf("Invalid character in input\n");
            return EXIT_FAILURE;
        }
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed %zu words:\n", word_count);
    for (size_t i = 0; i < word_count; i++) {
        words[i].text[words[i].length] = '\0';
        printf("%s", words[i].text);
        if (i < word_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    size_t unique_count = 0;
    for (size_t i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (size_t j = 0; j < i; j++) {
            if (strncmp(words[i].text, words[j].text, MAX_WORD_LEN) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            unique_count++;
        }
    }
    
    printf("Unique words: %zu\n", unique_count);
    
    if (word_count >= 2) {
        for (size_t i = 0; i < word_count - 1; i++) {
            for (size_t j = 0; j < word_count - i - 1; j++) {
                if (strncmp(words[j].text, words[j + 1].text, MAX_WORD_LEN) > 0) {
                    Word temp = words[j];
                    words[j] = words[j + 1];
                    words[j + 1] = temp;
                }
            }
        }
        
        printf("Sorted words: ");
        for (size_t i = 0; i < word_count; i++) {
            printf("%s", words[i].text);
            if (i < word_count - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
    
    return EXIT_SUCCESS;
}