//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && j < 63 && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            i++;
            j++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            
            for (k = 0; k < j; k++) {
                words[word_count][k] = tolower((unsigned char)words[word_count][k]);
            }
            
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: ");
    
    int unique_count = 0;
    for (i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            unique_count++;
            if (unique_count > 1) {
                printf(", ");
            }
            printf("%s", words[i]);
        }
    }
    printf("\n");
    
    printf("Word frequencies:\n");
    for (i = 0; i < word_count; i++) {
        int counted = 0;
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                counted = 1;
                break;
            }
        }
        if (!counted) {
            int freq = 1;
            for (j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    freq++;
                }
            }
            printf("%s: %d\n", words[i], freq);
        }
    }
    
    char longest_word[64] = "";
    char shortest_word[64] = "";
    size_t max_len = 0;
    size_t min_len = 64;
    
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
            strncpy(longest_word, words[i], sizeof(longest_word) - 1);
            longest_word[sizeof(longest_word) - 1] = '\0';
        }
        if (len < min_len) {
            min_len = len;
            strncpy(shortest_word, words[i], sizeof(shortest_word) - 1);
            shortest_word[sizeof(shortest_word) - 1] = '\0';
        }
    }
    
    printf("Longest word: %s (%zu characters)\n", longest_word, max_len);
    printf("Shortest word: %s (%zu characters)\n", shortest_word, min_len);
    
    return 0;
}