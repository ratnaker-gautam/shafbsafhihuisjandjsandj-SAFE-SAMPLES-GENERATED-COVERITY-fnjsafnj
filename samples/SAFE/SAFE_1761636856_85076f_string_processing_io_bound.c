//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char temp[MAX_INPUT_LEN];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nWord analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    int longest_len = 0;
    int shortest_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        total_chars += (int)word_len;
        
        if ((int)word_len > longest_len) {
            longest_len = (int)word_len;
        }
        if ((int)word_len < shortest_len) {
            shortest_len = (int)word_len;
        }
    }
    
    printf("Average word length: %.2f\n", (double)total_chars / word_count);
    printf("Longest word length: %d\n", longest_len);
    printf("Shortest word length: %d\n", shortest_len);
    
    printf("\nWords in reverse order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("\nWord frequency (case-insensitive):\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') continue;
        
        char lower_word[MAX_WORD_LEN];
        strncpy(lower_word, words[i], sizeof(lower_word) - 1);
        lower_word[sizeof(lower_word) - 1] = '\0';
        
        for (size_t j = 0; j < strlen(lower_word); j++) {
            lower_word[j] = tolower(lower_word[j]);
        }
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (words[j][0] == '\0') continue;
            
            char compare_word[MAX_WORD_LEN];
            strncpy(compare_word, words[j], sizeof(compare_word) - 1);
            compare_word[sizeof(compare_word) - 1] = '\0';
            
            for (size_t k = 0; k < strlen(compare_word); k++) {
                compare_word[k] = tolower(compare_word[k]);
            }
            
            if (strcmp(lower_word, compare_word) == 0) {
                count++;
                words[j][0] = '\0';
            }
        }
        
        printf("%s: %d\n", words[i], count);
    }
    
    return 0;
}