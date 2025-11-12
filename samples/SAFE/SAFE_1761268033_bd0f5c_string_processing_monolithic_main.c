//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text to analyze: ");
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
    
    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len >= MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            token = strtok(NULL, " \t\n\r");
            continue;
        }
        
        int valid_word = 1;
        for (size_t i = 0; i < token_len; i++) {
            if (!isalpha((unsigned char)token[i])) {
                valid_word = 0;
                break;
            }
        }
        
        if (valid_word && token_len > 0) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nWord analysis results:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    int longest_len = 0;
    int shortest_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        total_chars += len;
        
        if (len > longest_len) {
            longest_len = len;
        }
        if (len < shortest_len) {
            shortest_len = len;
        }
    }
    
    printf("Average word length: %.2f\n", (float)total_chars / word_count);
    printf("Longest word length: %d\n", longest_len);
    printf("Shortest word length: %d\n", shortest_len);
    
    printf("\nWords sorted by length:\n");
    for (int len = shortest_len; len <= longest_len; len++) {
        for (int i = 0; i < word_count; i++) {
            if (strlen(words[i]) == len) {
                printf("%s ", words[i]);
            }
        }
    }
    printf("\n");
    
    char search_word[MAX_WORD_LEN];
    printf("\nEnter a word to search for: ");
    if (fgets(search_word, sizeof(search_word), stdin) == NULL) {
        fprintf(stderr, "Error reading search word\n");
        return 1;
    }
    
    size_t search_len = strlen(search_word);
    if (search_len > 0 && search_word[search_len - 1] == '\n') {
        search_word[search_len - 1] = '\0';
        search_len--;
    }
    
    if (search_len == 0) {
        printf("No search word provided\n");
        return 1;
    }
    
    int found_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], search_word) == 0) {
            found_count++;
        }
    }
    
    if (found_count > 0) {
        printf("Word '%s' found %d time(s)\n", search_word, found_count);
    } else {
        printf("Word '%s' not found\n", search_word);
    }
    
    return 0;
}