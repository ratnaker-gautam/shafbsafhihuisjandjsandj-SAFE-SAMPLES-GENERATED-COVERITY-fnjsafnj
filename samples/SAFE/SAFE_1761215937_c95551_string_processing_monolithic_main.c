//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
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
            printf("%s: %zu\n", words[i], strlen(words[i]));
        }
        
        int longest_index = 0;
        int shortest_index = 0;
        for (int i = 1; i < word_count; i++) {
            if (strlen(words[i]) > strlen(words[longest_index])) {
                longest_index = i;
            }
            if (strlen(words[i]) < strlen(words[shortest_index])) {
                shortest_index = i;
            }
        }
        
        printf("\nLongest word: %s (%zu characters)\n", 
               words[longest_index], strlen(words[longest_index]));
        printf("Shortest word: %s (%zu characters)\n", 
               words[shortest_index], strlen(words[shortest_index]));
    }
    
    char processed[MAX_INPUT_LEN + 1];
    size_t proc_idx = 0;
    
    for (size_t i = 0; i < input_len && proc_idx < MAX_INPUT_LEN; i++) {
        if (isalpha((unsigned char)input[i])) {
            processed[proc_idx++] = toupper((unsigned char)input[i]);
        } else if (isspace((unsigned char)input[i])) {
            processed[proc_idx++] = ' ';
        }
    }
    processed[proc_idx] = '\0';
    
    printf("\nProcessed text (uppercase letters only):\n%s\n", processed);
    
    return 0;
}