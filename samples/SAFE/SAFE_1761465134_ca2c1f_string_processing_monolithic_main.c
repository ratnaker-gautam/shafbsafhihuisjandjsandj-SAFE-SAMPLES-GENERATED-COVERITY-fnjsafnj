//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
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
    
    char_count = input_len;
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
            int len = strlen(words[i]);
            printf("%s: %d characters\n", words[i], len);
        }
        
        int total_chars_in_words = 0;
        for (int i = 0; i < word_count; i++) {
            total_chars_in_words += strlen(words[i]);
        }
        
        if (word_count > 0) {
            double avg_length = (double)total_chars_in_words / word_count;
            printf("\nAverage word length: %.2f\n", avg_length);
        }
    }
    
    char processed[MAX_INPUT_LEN];
    int proc_index = 0;
    
    for (size_t i = 0; i < input_len && proc_index < MAX_INPUT_LEN - 1; i++) {
        if (isalpha((unsigned char)input[i])) {
            processed[proc_index++] = toupper((unsigned char)input[i]);
        } else if (isspace((unsigned char)input[i])) {
            processed[proc_index++] = ' ';
        }
    }
    processed[proc_index] = '\0';
    
    printf("\nProcessed text (uppercase letters only):\n%s\n", processed);
    
    return 0;
}