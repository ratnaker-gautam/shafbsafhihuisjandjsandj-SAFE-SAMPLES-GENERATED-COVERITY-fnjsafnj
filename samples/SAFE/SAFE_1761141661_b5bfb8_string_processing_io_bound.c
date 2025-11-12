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
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len <= MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    int total_chars = 0;
    int longest_word_len = 0;
    int shortest_word_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        int word_len = strlen(words[i]);
        total_chars += word_len;
        
        if (word_len > longest_word_len) {
            longest_word_len = word_len;
        }
        if (word_len < shortest_word_len) {
            shortest_word_len = word_len;
        }
        
        for (int j = 0; j < word_len; j++) {
            words[i][j] = toupper((unsigned char)words[i][j]);
        }
    }
    
    printf("Longest word length: %d\n", longest_word_len);
    printf("Shortest word length: %d\n", shortest_word_len);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0f);
    
    printf("\nWords in uppercase:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        int count = 1;
        if (strlen(words[i]) == 0) continue;
        
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                words[j][0] = '\0';
            }
        }
        printf("'%s': %d\n", words[i], count);
    }
    
    return 0;
}