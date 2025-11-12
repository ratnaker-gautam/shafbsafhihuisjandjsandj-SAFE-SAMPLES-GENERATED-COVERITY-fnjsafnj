//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORD_COUNT 512
#define MAX_WORD_LENGTH 64

int main() {
    char input[MAX_INPUT_LENGTH + 1];
    char words[MAX_WORD_COUNT][MAX_WORD_LENGTH + 1];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LENGTH);
    
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
    
    if (input_len > MAX_INPUT_LENGTH) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORD_COUNT) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len <= MAX_WORD_LENGTH) {
            strncpy(words[word_count], token, MAX_WORD_LENGTH);
            words[word_count][MAX_WORD_LENGTH] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nWord count: %d\n", word_count);
    printf("Words in reverse order:\n");
    
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int longest_length = 0;
    int shortest_length = MAX_WORD_LENGTH;
    
    for (int i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        total_chars += (int)word_len;
        
        if ((int)word_len > longest_length) {
            longest_length = (int)word_len;
        }
        if ((int)word_len < shortest_length) {
            shortest_length = (int)word_len;
        }
    }
    
    printf("Total characters in words: %d\n", total_chars);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    printf("Longest word length: %d\n", longest_length);
    printf("Shortest word length: %d\n", shortest_length);
    
    printf("\nWords containing only letters:\n");
    int letter_only_count = 0;
    for (int i = 0; i < word_count; i++) {
        int all_letters = 1;
        for (size_t j = 0; j < strlen(words[i]); j++) {
            if (!isalpha((unsigned char)words[i][j])) {
                all_letters = 0;
                break;
            }
        }
        if (all_letters) {
            printf("%s ", words[i]);
            letter_only_count++;
        }
    }
    
    if (letter_only_count > 0) {
        printf("\n");
    } else {
        printf("None\n");
    }
    
    return 0;
}