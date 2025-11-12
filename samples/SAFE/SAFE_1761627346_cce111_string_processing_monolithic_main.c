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
    
    char_count = (int)input_len;
    
    for (int i = 0; i < (int)input_len; i++) {
        if (input[i] == '\n') {
            line_count++;
        }
    }
    line_count++;
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len < sizeof(words[0])) {
            strncpy(words[word_count], token, sizeof(words[0]) - 1);
            words[word_count][sizeof(words[0]) - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord List:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord Lengths:\n");
        for (int i = 0; i < word_count; i++) {
            int len = (int)strlen(words[i]);
            printf("%s: %d characters\n", words[i], len);
        }
        
        int total_word_chars = 0;
        for (int i = 0; i < word_count; i++) {
            total_word_chars += (int)strlen(words[i]);
        }
        
        if (word_count > 0) {
            double avg_length = (double)total_word_chars / word_count;
            printf("\nAverage word length: %.2f\n", avg_length);
        }
    }
    
    char processed[MAX_INPUT_LEN + 1];
    int proc_idx = 0;
    
    for (int i = 0; i < (int)input_len && proc_idx < MAX_INPUT_LEN; i++) {
        if (isalpha((unsigned char)input[i])) {
            processed[proc_idx] = toupper((unsigned char)input[i]);
            proc_idx++;
        } else if (isspace((unsigned char)input[i])) {
            if (proc_idx > 0 && processed[proc_idx - 1] != ' ') {
                processed[proc_idx] = ' ';
                proc_idx++;
            }
        }
    }
    
    if (proc_idx > 0 && processed[proc_idx - 1] == ' ') {
        proc_idx--;
    }
    processed[proc_idx] = '\0';
    
    if (proc_idx > 0) {
        printf("\nProcessed Text (uppercase, single spaces):\n%s\n", processed);
    }
    
    return 0;
}