//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) ((c) >= 'a' && (c) <= 'z' ? (c) - 32 : (c))
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int main(void) {
    char input[MAX_INPUT_LEN];
    char processed[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
    
    int in_word = 0;
    int proc_idx = 0;
    int capitalize_next = 1;
    
    for (size_t i = 0; i < input_len && proc_idx < (int)sizeof(processed) - 1; i++) {
        char c = input[i];
        
        if (IS_WHITESPACE(c)) {
            if (in_word) {
                processed[proc_idx++] = ' ';
                in_word = 0;
            }
            capitalize_next = IS_TERMINATOR(input[i - 1]) ? 1 : 0;
        } else if (IS_TERMINATOR(c)) {
            processed[proc_idx++] = c;
            in_word = 0;
            capitalize_next = 1;
        } else if (isalpha((unsigned char)c)) {
            if (capitalize_next) {
                processed[proc_idx++] = TO_UPPER(c);
                capitalize_next = 0;
            } else {
                processed[proc_idx++] = TO_LOWER(c);
            }
            in_word = 1;
        } else {
            processed[proc_idx++] = c;
            in_word = 1;
        }
    }
    
    if (proc_idx > 0 && IS_WHITESPACE(processed[proc_idx - 1])) {
        proc_idx--;
    }
    processed[proc_idx] = '\0';
    
    printf("Processed text: %s\n", processed);
    
    char temp[MAX_INPUT_LEN];
    strncpy(temp, processed, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i].word, token) == 0) {
                    words[i].count++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strncpy(words[word_count].word, token, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
        token = strtok(NULL, " \t\n");
    }
    
    printf("\nWord frequency:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}