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
    int i, j;
    
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
    
    i = 0;
    while (input[i] != '\0' && word_count < MAX_WORDS) {
        while (input[i] != '\0' && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        j = 0;
        while (input[i] != '\0' && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j >= MAX_WORD_LEN - 1) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nWords found (%d):\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words...\n");
    
    int total_chars = 0;
    int longest_word_len = 0;
    int shortest_word_len = MAX_WORD_LEN;
    
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        total_chars += (int)word_len;
        
        if ((int)word_len > longest_word_len) {
            longest_word_len = (int)word_len;
        }
        if ((int)word_len < shortest_word_len) {
            shortest_word_len = (int)word_len;
        }
        
        for (j = 0; words[i][j] != '\0'; j++) {
            words[i][j] = (char)toupper((unsigned char)words[i][j]);
        }
    }
    
    printf("\nProcessed words (uppercase):\n");
    for (i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", total_chars);
    printf("Longest word length: %d\n", longest_word_len);
    printf("Shortest word length: %d\n", shortest_word_len);
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("Average word length: %.2f\n", avg_length);
    }
    
    return 0;
}