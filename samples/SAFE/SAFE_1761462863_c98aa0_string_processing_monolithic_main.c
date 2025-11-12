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
        printf("Empty input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = (int)input_len;
    line_count = 1;
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            if (token_len > MAX_WORD_LEN) {
                fprintf(stderr, "Word too long: %s\n", token);
                return 1;
            }
            
            int valid_word = 1;
            for (size_t i = 0; i < token_len; i++) {
                if (!isalpha((unsigned char)token[i])) {
                    valid_word = 0;
                    break;
                }
            }
            
            if (valid_word) {
                strncpy(words[word_count], token, MAX_WORD_LEN);
                words[word_count][MAX_WORD_LEN] = '\0';
                word_count++;
            }
            
            token = strtok(NULL, " \t\n");
        }
    }
    
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nValid words found:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord lengths:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %zu characters\n", words[i], strlen(words[i]));
        }
        
        if (word_count >= 2) {
            printf("\nComparing first two words:\n");
            int cmp = strcmp(words[0], words[1]);
            if (cmp < 0) {
                printf("'%s' comes before '%s'\n", words[0], words[1]);
            } else if (cmp > 0) {
                printf("'%s' comes after '%s'\n", words[0], words[1]);
            } else {
                printf("'%s' and '%s' are identical\n", words[0], words[1]);
            }
        }
    }
    
    return 0;
}