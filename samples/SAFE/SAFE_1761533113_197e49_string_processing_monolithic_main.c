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
    
    char temp[MAX_INPUT_LEN];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n");
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
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    
    if (word_count > 0) {
        printf("\nWord frequency analysis:\n");
        
        int frequencies[MAX_WORDS] = {0};
        int processed[MAX_WORDS] = {0};
        
        for (int i = 0; i < word_count; i++) {
            if (processed[i]) continue;
            
            frequencies[i] = 1;
            processed[i] = 1;
            
            for (int j = i + 1; j < word_count; j++) {
                if (!processed[j] && strcmp(words[i], words[j]) == 0) {
                    frequencies[i]++;
                    processed[j] = 1;
                }
            }
        }
        
        for (int i = 0; i < word_count; i++) {
            if (frequencies[i] > 0) {
                printf("'%s': %d\n", words[i], frequencies[i]);
            }
        }
        
        printf("\nLongest word(s): ");
        int max_len = 0;
        for (int i = 0; i < word_count; i++) {
            int len = (int)strlen(words[i]);
            if (len > max_len) {
                max_len = len;
            }
        }
        
        int first = 1;
        for (int i = 0; i < word_count; i++) {
            if ((int)strlen(words[i]) == max_len) {
                if (!first) printf(", ");
                printf("'%s'", words[i]);
                first = 0;
            }
        }
        printf(" (%d characters)\n", max_len);
    }
    
    return 0;
}