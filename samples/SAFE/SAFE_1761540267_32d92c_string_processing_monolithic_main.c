//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
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
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace(input[i]) && j < 63) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    for (i = 0; i < word_count; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                char temp[64];
                strncpy(temp, words[i], sizeof(temp));
                temp[63] = '\0';
                strncpy(words[i], words[j], sizeof(words[i]));
                words[i][63] = '\0';
                strncpy(words[j], temp, sizeof(words[j]));
                words[j][63] = '\0';
            }
        }
    }
    
    printf("Sorted words:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord frequency:\n");
    i = 0;
    while (i < word_count) {
        int count = 1;
        j = i + 1;
        while (j < word_count && strcmp(words[i], words[j]) == 0) {
            count++;
            j++;
        }
        printf("%s: %d\n", words[i], count);
        i = j;
    }
    
    char longest_word[64] = "";
    char shortest_word[64] = "";
    strncpy(shortest_word, words[0], sizeof(shortest_word));
    shortest_word[63] = '\0';
    
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > strlen(longest_word)) {
            strncpy(longest_word, words[i], sizeof(longest_word));
            longest_word[63] = '\0';
        }
        if (len < strlen(shortest_word)) {
            strncpy(shortest_word, words[i], sizeof(shortest_word));
            shortest_word[63] = '\0';
        }
    }
    
    printf("\nLongest word: %s (%zu characters)\n", longest_word, strlen(longest_word));
    printf("Shortest word: %s (%zu characters)\n", shortest_word, strlen(shortest_word));
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
    }
    
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    
    return 0;
}