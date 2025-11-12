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
    int word_lengths[MAX_WORDS];
    
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
        if (token_len > MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            return 1;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN);
        words[word_count][MAX_WORD_LEN] = '\0';
        
        int clean_len = 0;
        for (size_t i = 0; i < token_len && clean_len < MAX_WORD_LEN; i++) {
            if (isalnum((unsigned char)token[i])) {
                words[word_count][clean_len++] = tolower((unsigned char)token[i]);
            }
        }
        words[word_count][clean_len] = '\0';
        
        if (clean_len > 0) {
            word_lengths[word_count] = clean_len;
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nWord Analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    int shortest = word_lengths[0];
    int longest = word_lengths[0];
    
    for (int i = 0; i < word_count; i++) {
        total_chars += word_lengths[i];
        if (word_lengths[i] < shortest) shortest = word_lengths[i];
        if (word_lengths[i] > longest) longest = word_lengths[i];
    }
    
    printf("Average word length: %.2f\n", (float)total_chars / word_count);
    printf("Shortest word length: %d\n", shortest);
    printf("Longest word length: %d\n", longest);
    
    printf("\nWords sorted by length:\n");
    for (int len = shortest; len <= longest; len++) {
        for (int i = 0; i < word_count; i++) {
            if (word_lengths[i] == len) {
                printf("%s (%d)\n", words[i], len);
            }
        }
    }
    
    printf("\nWord frequency:\n");
    int processed[MAX_WORDS] = {0};
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("%s: %d\n", words[i], count);
        processed[i] = 1;
    }
    
    return 0;
}