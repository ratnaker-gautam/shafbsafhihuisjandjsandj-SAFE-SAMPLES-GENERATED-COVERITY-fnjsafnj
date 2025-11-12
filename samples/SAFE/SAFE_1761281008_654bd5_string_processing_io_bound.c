//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
        printf("No input provided\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < 64) {
            strncpy(words[word_count], token, 63);
            words[word_count][63] = '\0';
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
    int longest_len = 0;
    int shortest_len = 1000;
    
    for (int i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        total_chars += (int)word_len;
        
        if ((int)word_len > longest_len) {
            longest_len = (int)word_len;
        }
        
        if ((int)word_len < shortest_len) {
            shortest_len = (int)word_len;
        }
        
        for (size_t j = 0; j < word_len; j++) {
            words[i][j] = (char)toupper((unsigned char)words[i][j]);
        }
    }
    
    printf("Longest word length: %d\n", longest_len);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    
    printf("\nProcessed words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                words[j][0] = '\0';
            }
        }
        
        printf("'%s' appears %d time(s)\n", words[i], count);
    }
    
    return 0;
}