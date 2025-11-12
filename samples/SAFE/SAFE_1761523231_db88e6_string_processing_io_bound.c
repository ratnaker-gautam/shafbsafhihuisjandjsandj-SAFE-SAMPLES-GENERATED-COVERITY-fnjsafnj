//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN + 1];
    char *words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to process: ");
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
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            words[word_count] = malloc(token_len + 1);
            if (words[word_count] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                return 1;
            }
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Processing %d words...\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        char *word = words[i];
        size_t word_len = strlen(word);
        
        for (size_t j = 0; j < word_len; j++) {
            word[j] = toupper((unsigned char)word[j]);
        }
        
        printf("Word %d: %s (length: %zu)\n", i + 1, word, word_len);
    }
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i] == NULL) continue;
        
        int freq = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (words[j] != NULL && strcmp(words[i], words[j]) == 0) {
                freq++;
                free(words[j]);
                words[j] = NULL;
            }
        }
        
        if (freq > 0) {
            printf("'%s' appears %d time(s)\n", words[i], freq);
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        if (words[i] != NULL) {
            free(words[i]);
        }
    }
    
    return 0;
}