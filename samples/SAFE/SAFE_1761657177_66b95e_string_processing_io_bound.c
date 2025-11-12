//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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
    
    char current_word[MAX_WORD_LEN + 1];
    int word_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalpha((unsigned char)c)) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            
            if (word_len < MAX_WORD_LEN) {
                current_word[word_len++] = tolower((unsigned char)c);
            }
        } else {
            if (in_word && word_len > 0 && word_count < MAX_WORDS) {
                current_word[word_len] = '\0';
                strncpy(words[word_count], current_word, MAX_WORD_LEN);
                words[word_count][MAX_WORD_LEN] = '\0';
                word_count++;
            }
            in_word = 0;
            word_len = 0;
        }
        
        if (c == '\n' || c == '\0') {
            line_count++;
        }
        
        if (c != '\0') {
            char_count++;
        }
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord frequency analysis:\n");
        
        for (int i = 0; i < word_count; i++) {
            if (words[i][0] == '\0') continue;
            
            int freq = 1;
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    freq++;
                    words[j][0] = '\0';
                }
            }
            
            printf("'%s': %d\n", words[i], freq);
        }
    }
    
    printf("\nProcessed text (lowercase, words only):\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i][0] != '\0') {
            printf("%s ", words[i]);
        }
    }
    printf("\n");
    
    return 0;
}