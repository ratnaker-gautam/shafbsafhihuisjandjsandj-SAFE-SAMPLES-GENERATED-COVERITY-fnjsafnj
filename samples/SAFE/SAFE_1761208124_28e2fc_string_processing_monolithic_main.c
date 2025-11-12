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
    int total_chars = 0;
    
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
    int current_word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '\'' || c == '-') {
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
                    total_chars += current_word_len;
                    word_count++;
                }
                in_word = 0;
                current_word_len = 0;
            }
        }
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters in words: %d\n", total_chars);
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("Average word length: %.2f\n", avg_length);
        
        int max_len = 0;
        int min_len = 1000;
        
        for (int i = 0; i < word_count; i++) {
            if (word_lengths[i] > max_len) {
                max_len = word_lengths[i];
            }
            if (word_lengths[i] < min_len) {
                min_len = word_lengths[i];
            }
        }
        
        printf("Longest word length: %d\n", max_len);
        printf("Shortest word length: %d\n", min_len);
        
        printf("\nWord frequency by length:\n");
        int length_freq[64] = {0};
        
        for (int i = 0; i < word_count; i++) {
            if (word_lengths[i] < 64) {
                length_freq[word_lengths[i]]++;
            }
        }
        
        for (int i = 1; i < 64; i++) {
            if (length_freq[i] > 0) {
                printf("Length %d: %d words\n", i, length_freq[i]);
            }
        }
        
        printf("\nWords (alphabetically sorted):\n");
        for (int i = 0; i < word_count - 1; i++) {
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) > 0) {
                    char temp[64];
                    strcpy(temp, words[i]);
                    strcpy(words[i], words[j]);
                    strcpy(words[j], temp);
                    
                    int temp_len = word_lengths[i];
                    word_lengths[i] = word_lengths[j];
                    word_lengths[j] = temp_len;
                }
            }
        }
        
        for (int i = 0; i < word_count; i++) {
            printf("%s\n", words[i]);
        }
    }
    
    return 0;
}