//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided.\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = len;
    line_count = 1;
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            if (token_len <= MAX_WORD_LEN) {
                strncpy(words[word_count], token, MAX_WORD_LEN);
                words[word_count][MAX_WORD_LEN] = '\0';
                word_count++;
            } else {
                fprintf(stderr, "Word too long: %s\n", token);
                return 1;
            }
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (word_count == MAX_WORDS && token != NULL) {
        fprintf(stderr, "Too many words\n");
        return 1;
    }
    
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWords found:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord lengths:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %zu characters\n", words[i], strlen(words[i]));
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
    
    printf("\nProcessing complete.\n");
    return 0;
}