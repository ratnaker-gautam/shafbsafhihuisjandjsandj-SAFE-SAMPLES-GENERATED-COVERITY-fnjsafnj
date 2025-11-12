//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_INPUT_LEN / 2][MAX_WORD_LEN + 1];
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
    
    int in_word = 0;
    int word_start = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
            }
        } else {
            if (in_word) {
                int word_length = i - word_start;
                if (word_length > MAX_WORD_LEN) {
                    fprintf(stderr, "Word too long\n");
                    return 1;
                }
                
                if (word_count >= MAX_INPUT_LEN / 2) {
                    fprintf(stderr, "Too many words\n");
                    return 1;
                }
                
                strncpy(words[word_count], &input[word_start], word_length);
                words[word_count][word_length] = '\0';
                word_count++;
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nFound %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words...\n");
    
    int total_chars = 0;
    int longest_word = 0;
    int shortest_word = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        total_chars += len;
        
        if (len > longest_word) {
            longest_word = len;
        }
        if (len < shortest_word) {
            shortest_word = len;
        }
        
        for (int j = 0; j < len; j++) {
            words[i][j] = toupper((unsigned char)words[i][j]);
        }
    }
    
    printf("\nProcessed words (uppercase):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    double avg_length = word_count > 0 ? (double)total_chars / word_count : 0.0;
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", total_chars);
    printf("Average word length: %.2f\n", avg_length);
    printf("Longest word: %d characters\n", longest_word);
    printf("Shortest word: %d characters\n", shortest_word);
    
    return 0;
}