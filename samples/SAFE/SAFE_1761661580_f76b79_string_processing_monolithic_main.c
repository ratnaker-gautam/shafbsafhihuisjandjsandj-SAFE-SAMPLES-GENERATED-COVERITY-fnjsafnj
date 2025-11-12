//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
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
    int word_lengths[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    int in_word = 0;
    int current_word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c)) {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            
            if (current_word_len < 63 && word_count < MAX_WORDS) {
                words[word_count][current_word_len] = c;
                current_word_len++;
            }
        } else {
            if (in_word) {
                if (current_word_len > 0 && word_count < MAX_WORDS) {
                    words[word_count][current_word_len] = '\0';
                    word_lengths[word_count] = current_word_len;
                    word_count++;
                }
                in_word = 0;
                current_word_len = 0;
            }
        }
    }
    
    printf("\nText analysis results:\n");
    printf("Total words: %d\n", word_count);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    int total_chars = 0;
    int shortest_len = 64;
    int longest_len = 0;
    
    for (int i = 0; i < word_count; i++) {
        int len = word_lengths[i];
        total_chars += len;
        
        if (len < shortest_len) {
            shortest_len = len;
        }
        if (len > longest_len) {
            longest_len = len;
        }
    }
    
    printf("Average word length: %.2f\n", (double)total_chars / word_count);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Longest word length: %d\n", longest_len);
    
    printf("\nWord frequency by length:\n");
    int length_counts[64] = {0};
    
    for (int i = 0; i < word_count; i++) {
        int len = word_lengths[i];
        if (len >= 0 && len < 64) {
            length_counts[len]++;
        }
    }
    
    for (int len = 1; len < 64; len++) {
        if (length_counts[len] > 0) {
            printf("%d-letter words: %d\n", len, length_counts[len]);
        }
    }
    
    printf("\nFirst 10 words found:\n");
    int display_count = word_count < 10 ? word_count : 10;
    for (int i = 0; i < display_count; i++) {
        printf("%d. %s\n", i + 1, words[i]);
    }
    
    return 0;
}