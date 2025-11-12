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
    
    for (int i = 0; i < input_len; i++) {
        if (input[i] == '\n') {
            line_count++;
        }
    }
    line_count++;
    
    char current_word[MAX_WORD_LEN + 1];
    int word_len = 0;
    int in_word = 0;
    
    for (int i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '_') {
            if (word_len < MAX_WORD_LEN) {
                current_word[word_len++] = c;
            }
            in_word = 1;
        } else {
            if (in_word && word_len > 0 && word_count < MAX_WORDS) {
                current_word[word_len] = '\0';
                strncpy(words[word_count], current_word, MAX_WORD_LEN);
                words[word_count][MAX_WORD_LEN] = '\0';
                word_count++;
                word_len = 0;
            }
            in_word = 0;
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
    
    char search_word[MAX_WORD_LEN + 1];
    printf("\nEnter a word to search for: ");
    if (fgets(search_word, sizeof(search_word), stdin) != NULL) {
        size_t search_len = strlen(search_word);
        if (search_len > 0 && search_word[search_len - 1] == '\n') {
            search_word[search_len - 1] = '\0';
            search_len--;
        }
        
        if (search_len > 0) {
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i], search_word) == 0) {
                    found++;
                }
            }
            printf("The word '%s' appears %d time(s)\n", search_word, found);
        }
    }
    
    return 0;
}