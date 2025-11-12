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
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
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
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    int total_length = 0;
    int longest_length = 0;
    int shortest_length = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        total_length += len;
        
        if (len > longest_length) {
            longest_length = len;
        }
        if (len < shortest_length) {
            shortest_length = len;
        }
        
        for (int j = 0; j < len; j++) {
            words[i][j] = toupper((unsigned char)words[i][j]);
        }
    }
    
    printf("\nProcessed words (uppercase):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_length / word_count : 0.0);
    printf("Longest word length: %d\n", longest_length);
    printf("Shortest word length: %d\n", shortest_length);
    
    printf("\nWords sorted alphabetically:\n");
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                char temp[MAX_WORD_LEN + 1];
                strcpy(temp, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], temp);
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}