//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char temp_word[MAX_WORD_LEN];
    int temp_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        
        if (IS_ALPHA(c)) {
            if (temp_len >= MAX_WORD_LEN - 1) {
                fprintf(stderr, "Word too long\n");
                return 1;
            }
            
            temp_word[temp_len++] = TO_LOWER(c);
            in_word = 1;
        } else if (IS_WHITESPACE(c) || c == '\0') {
            if (in_word) {
                temp_word[temp_len] = '\0';
                
                if (temp_len > 0) {
                    int found = 0;
                    for (int j = 0; j < word_count; j++) {
                        if (strcmp(words[j].word, temp_word) == 0) {
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
                        
                        if (strlen(temp_word) < sizeof(words[word_count].word)) {
                            strcpy(words[word_count].word, temp_word);
                            words[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
                
                temp_len = 0;
                in_word = 0;
            }
            
            if (c == '\0') {
                break;
            }
        }
    }
    
    if (in_word && temp_len > 0) {
        temp_word[temp_len] = '\0';
        
        int found = 0;
        for (int j = 0; j < word_count; j++) {
            if (strcmp(words[j].word, temp_word) == 0) {
                words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (word_count < MAX_WORDS && strlen(temp_word) < sizeof(words[word_count].word)) {
                strcpy(words[word_count].word, temp_word);
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}