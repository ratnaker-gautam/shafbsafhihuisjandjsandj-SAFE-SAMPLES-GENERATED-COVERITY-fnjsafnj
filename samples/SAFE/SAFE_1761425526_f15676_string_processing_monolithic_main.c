//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
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
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len < sizeof(words[0])) {
            strncpy(words[word_count], token, sizeof(words[0]) - 1);
            words[word_count][sizeof(words[0]) - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    
    if (word_count > 0) {
        printf("\nWord frequency analysis:\n");
        
        for (int i = 0; i < word_count; i++) {
            for (size_t j = 0; j < strlen(words[i]); j++) {
                words[i][j] = tolower((unsigned char)words[i][j]);
            }
        }
        
        int unique_count = 0;
        for (int i = 0; i < word_count; i++) {
            if (words[i][0] == '\0') continue;
            
            int count = 1;
            for (int j = i + 1; j < word_count; j++) {
                if (words[j][0] != '\0' && strcmp(words[i], words[j]) == 0) {
                    count++;
                    words[j][0] = '\0';
                }
            }
            
            if (count > 0) {
                printf("'%s': %d\n", words[i], count);
                unique_count++;
            }
        }
        
        printf("Unique words: %d\n", unique_count);
        
        int longest_index = 0;
        int longest_len = 0;
        for (int i = 0; i < word_count; i++) {
            int len = strlen(words[i]);
            if (len > longest_len && words[i][0] != '\0') {
                longest_len = len;
                longest_index = i;
            }
        }
        
        if (longest_len > 0) {
            printf("Longest word: '%s' (%d characters)\n", words[longest_index], longest_len);
        }
    }
    
    return 0;
}