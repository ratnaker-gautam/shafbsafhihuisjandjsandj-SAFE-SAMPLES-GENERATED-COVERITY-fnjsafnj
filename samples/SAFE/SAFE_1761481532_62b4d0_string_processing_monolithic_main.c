//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int word_lengths[MAX_WORDS];
    
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
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int current_word_len = 0;
    char current_word[64];
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '_') {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            
            if (current_word_len < 63) {
                current_word[current_word_len++] = c;
            }
        } else {
            if (in_word && current_word_len > 0) {
                if (word_count < MAX_WORDS) {
                    current_word[current_word_len] = '\0';
                    strncpy(words[word_count], current_word, 63);
                    words[word_count][63] = '\0';
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
    
    if (word_count > 0) {
        int total_chars = 0;
        int longest_len = 0;
        int shortest_len = 1000;
        
        for (int i = 0; i < word_count; i++) {
            total_chars += word_lengths[i];
            if (word_lengths[i] > longest_len) {
                longest_len = word_lengths[i];
            }
            if (word_lengths[i] < shortest_len) {
                shortest_len = word_lengths[i];
            }
        }
        
        double avg_length = (double)total_chars / word_count;
        printf("Average word length: %.2f\n", avg_length);
        printf("Longest word length: %d\n", longest_len);
        printf("Shortest word length: %d\n", shortest_len);
        
        printf("\nWord frequency by length:\n");
        int length_freq[64] = {0};
        for (int i = 0; i < word_count; i++) {
            if (word_lengths[i] < 64) {
                length_freq[word_lengths[i]]++;
            }
        }
        
        for (int i = 1; i < 64; i++) {
            if (length_freq[i] > 0) {
                printf("%d-letter words: %d\n", i, length_freq[i]);
            }
        }
        
        printf("\nWords (longest first):\n");
        for (int len = longest_len; len >= shortest_len; len--) {
            for (int i = 0; i < word_count; i++) {
                if (word_lengths[i] == len) {
                    printf("%s\n", words[i]);
                }
            }
        }
    }
    
    return 0;
}