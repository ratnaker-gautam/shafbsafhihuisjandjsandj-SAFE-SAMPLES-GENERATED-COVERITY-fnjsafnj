//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int char_count = 0;
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    
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
    
    char_count = (int)input_len;
    line_count = 1;
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord frequency analysis:\n");
        
        int processed[MAX_WORDS] = {0};
        for (int i = 0; i < word_count; i++) {
            if (processed[i]) continue;
            
            int count = 1;
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    count++;
                    processed[j] = 1;
                }
            }
            
            printf("'%s': %d\n", words[i], count);
            processed[i] = 1;
        }
        
        printf("\nWords in alphabetical order:\n");
        char temp[MAX_WORD_LEN];
        for (int i = 0; i < word_count - 1; i++) {
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) > 0) {
                    strncpy(temp, words[i], sizeof(temp) - 1);
                    temp[sizeof(temp) - 1] = '\0';
                    strncpy(words[i], words[j], MAX_WORD_LEN - 1);
                    words[i][MAX_WORD_LEN - 1] = '\0';
                    strncpy(words[j], temp, MAX_WORD_LEN - 1);
                    words[j][MAX_WORD_LEN - 1] = '\0';
                }
            }
        }
        
        for (int i = 0; i < word_count; i++) {
            printf("%s\n", words[i]);
        }
    }
    
    return 0;
}