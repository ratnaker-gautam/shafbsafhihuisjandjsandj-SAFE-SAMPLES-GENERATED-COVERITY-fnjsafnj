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
    
    char temp_word[MAX_WORD_LEN + 1];
    int temp_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c)) {
            if (!in_word) {
                in_word = 1;
                temp_len = 0;
            }
            
            if (temp_len < MAX_WORD_LEN) {
                temp_word[temp_len++] = c;
            }
        } else {
            if (in_word) {
                if (temp_len > 0 && word_count < MAX_WORDS) {
                    temp_word[temp_len] = '\0';
                    strncpy(words[word_count], temp_word, MAX_WORD_LEN);
                    words[word_count][MAX_WORD_LEN] = '\0';
                    word_count++;
                }
                in_word = 0;
                temp_len = 0;
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
        
        int total_word_chars = 0;
        for (int i = 0; i < word_count; i++) {
            total_word_chars += strlen(words[i]);
        }
        
        if (word_count > 0) {
            double avg_length = (double)total_word_chars / word_count;
            printf("\nAverage word length: %.2f\n", avg_length);
        }
    }
    
    char search_term[MAX_WORD_LEN + 1];
    printf("\nEnter a word to search for: ");
    
    if (fgets(search_term, sizeof(search_term), stdin) != NULL) {
        size_t search_len = strlen(search_term);
        if (search_len > 0 && search_term[search_len - 1] == '\n') {
            search_term[search_len - 1] = '\0';
            search_len--;
        }
        
        if (search_len > 0) {
            int found_count = 0;
            printf("Searching for '%s':\n", search_term);
            
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i], search_term) == 0) {
                    printf("Found at position %d\n", i + 1);
                    found_count++;
                }
            }
            
            if (found_count == 0) {
                printf("Word not found\n");
            } else {
                printf("Total occurrences: %d\n", found_count);
            }
        }
    }
    
    return 0;
}