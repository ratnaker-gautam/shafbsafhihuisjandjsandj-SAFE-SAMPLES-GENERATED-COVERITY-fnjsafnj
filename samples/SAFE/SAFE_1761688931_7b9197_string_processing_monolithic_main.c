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
    int word_lengths[MAX_WORDS] = {0};
    
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
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int current_word_start = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                current_word_start = i;
            }
        } else {
            if (in_word) {
                int word_len = i - current_word_start;
                
                if (word_count >= MAX_WORDS) {
                    fprintf(stderr, "Too many words\n");
                    return 1;
                }
                
                if (word_len >= 64) {
                    fprintf(stderr, "Word too long\n");
                    return 1;
                }
                
                strncpy(words[word_count], &input[current_word_start], word_len);
                words[word_count][word_len] = '\0';
                word_lengths[word_count] = word_len;
                word_count++;
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nWord analysis results:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    int shortest_len = word_lengths[0];
    int longest_len = word_lengths[0];
    
    for (int i = 0; i < word_count; i++) {
        total_chars += word_lengths[i];
        
        if (word_lengths[i] < shortest_len) {
            shortest_len = word_lengths[i];
        }
        if (word_lengths[i] > longest_len) {
            longest_len = word_lengths[i];
        }
    }
    
    double avg_length = (double)total_chars / word_count;
    printf("Average word length: %.2f\n", avg_length);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Longest word length: %d\n", longest_len);
    
    printf("\nWord frequency by length:\n");
    int length_freq[65] = {0};
    
    for (int i = 0; i < word_count; i++) {
        if (word_lengths[i] >= 0 && word_lengths[i] < 65) {
            length_freq[word_lengths[i]]++;
        }
    }
    
    for (int i = 1; i < 65; i++) {
        if (length_freq[i] > 0) {
            printf("%d-letter words: %d\n", i, length_freq[i]);
        }
    }
    
    printf("\nWords sorted by length:\n");
    int indices[MAX_WORDS];
    for (int i = 0; i < word_count; i++) {
        indices[i] = i;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (word_lengths[indices[j]] > word_lengths[indices[j + 1]]) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        printf("%s (%d)\n", words[indices[i]], word_lengths[indices[i]]);
    }
    
    return 0;
}