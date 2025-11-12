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
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = input_len;
    line_count = 1;
    
    char temp[MAX_INPUT_LEN + 1];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len <= MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord list:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        char longest_word[MAX_WORD_LEN + 1] = "";
        char shortest_word[MAX_WORD_LEN + 1] = "";
        int longest_len = 0;
        int shortest_len = MAX_WORD_LEN + 1;
        
        for (int i = 0; i < word_count; i++) {
            int current_len = strlen(words[i]);
            if (current_len > longest_len) {
                longest_len = current_len;
                strncpy(longest_word, words[i], MAX_WORD_LEN);
                longest_word[MAX_WORD_LEN] = '\0';
            }
            if (current_len < shortest_len && current_len > 0) {
                shortest_len = current_len;
                strncpy(shortest_word, words[i], MAX_WORD_LEN);
                shortest_word[MAX_WORD_LEN] = '\0';
            }
        }
        
        printf("\nLongest word: %s (%d characters)\n", longest_word, longest_len);
        if (shortest_len <= MAX_WORD_LEN) {
            printf("Shortest word: %s (%d characters)\n", shortest_word, shortest_len);
        }
    }
    
    char processed[MAX_INPUT_LEN + 1];
    int proc_index = 0;
    for (size_t i = 0; i < input_len && proc_index < MAX_INPUT_LEN; i++) {
        if (isalpha((unsigned char)input[i])) {
            processed[proc_index++] = toupper((unsigned char)input[i]);
        } else if (isspace((unsigned char)input[i])) {
            processed[proc_index++] = ' ';
        }
    }
    processed[proc_index] = '\0';
    
    if (proc_index > 0) {
        printf("\nProcessed text (uppercase letters only):\n%s\n", processed);
    }
    
    return 0;
}