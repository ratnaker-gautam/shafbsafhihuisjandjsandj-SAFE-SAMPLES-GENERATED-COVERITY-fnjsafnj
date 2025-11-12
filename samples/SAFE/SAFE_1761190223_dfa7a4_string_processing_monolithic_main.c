//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int word_lengths[MAX_WORDS];
    
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
        printf("Empty input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char current_word[MAX_WORD_LEN + 1];
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalpha((unsigned char)c) && word_pos < MAX_WORD_LEN) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            current_word[word_pos++] = tolower((unsigned char)c);
        } else {
            if (in_word) {
                if (word_pos > 0 && word_count < MAX_WORDS) {
                    current_word[word_pos] = '\0';
                    strncpy(words[word_count], current_word, MAX_WORD_LEN);
                    words[word_count][MAX_WORD_LEN] = '\0';
                    word_lengths[word_count] = word_pos;
                    word_count++;
                    char_count += word_pos;
                }
                in_word = 0;
                word_pos = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters in words: %d\n", char_count);
    
    if (word_count > 0) {
        double avg_length = (double)char_count / word_count;
        printf("Average word length: %.2f\n", avg_length);
    }
    
    printf("\nWord frequency analysis:\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i][0] != '\0') {
            int freq = 1;
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    freq++;
                    words[j][0] = '\0';
                }
            }
            printf("'%s': length %d, frequency %d\n", words[i], word_lengths[i], freq);
        }
    }
    
    printf("\nSorted by length:\n");
    int sorted_indices[MAX_WORDS];
    for (int i = 0; i < word_count; i++) {
        sorted_indices[i] = i;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (word_lengths[sorted_indices[i]] > word_lengths[sorted_indices[j]]) {
                int temp = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        int idx = sorted_indices[i];
        if (words[idx][0] != '\0') {
            printf("'%s' (%d)\n", words[idx], word_lengths[idx]);
        }
    }
    
    return 0;
}