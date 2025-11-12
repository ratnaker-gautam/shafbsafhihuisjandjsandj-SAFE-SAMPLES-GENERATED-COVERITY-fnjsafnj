//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
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
    
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (c == '\0' || IS_WHITESPACE(c)) {
            if (in_word && word_pos > 0) {
                current_word[word_pos] = '\0';
                
                int found = 0;
                for (int j = 0; j < word_count; j++) {
                    if (strcmp(current_word, words[j].word) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    if (word_count >= MAX_WORDS) {
                        fprintf(stderr, "Too many unique words\n");
                        return 1;
                    }
                    if (strlen(current_word) >= MAX_WORD_LEN) {
                        fprintf(stderr, "Word too long\n");
                        return 1;
                    }
                    strcpy(words[word_count].word, current_word);
                    words[word_count].count = 1;
                    word_count++;
                }
                
                word_pos = 0;
                in_word = 0;
            }
        } else {
            if (word_pos >= MAX_WORD_LEN - 1) {
                fprintf(stderr, "Word too long\n");
                return 1;
            }
            
            char processed_char = TO_LOWER(c);
            if (processed_char >= 'a' && processed_char <= 'z') {
                current_word[word_pos++] = processed_char;
                in_word = 1;
            }
        }
    }
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}