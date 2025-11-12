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
        
        if (isalnum((unsigned char)c) || c == '_') {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            
            if (word_pos < MAX_WORD_LEN) {
                current_word[word_pos++] = c;
            }
        } else {
            if (in_word) {
                if (word_pos > 0) {
                    current_word[word_pos] = '\0';
                    
                    if (word_count < MAX_WORDS) {
                        strncpy(words[word_count], current_word, MAX_WORD_LEN);
                        words[word_count][MAX_WORD_LEN] = '\0';
                        word_lengths[word_count] = word_pos;
                        word_count++;
                    }
                }
                in_word = 0;
                word_pos = 0;
            }
        }
    }
    
    printf("\nText analysis results:\n");
    printf("Total characters: %zu\n", input_len);
    printf("Words found: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord list:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d. '%s' (length: %d)\n", i + 1, words[i], word_lengths[i]);
        }
        
        int total_chars_in_words = 0;
        int longest_word_index = 0;
        int shortest_word_index = 0;
        
        for (int i = 0; i < word_count; i++) {
            total_chars_in_words += word_lengths[i];
            
            if (word_lengths[i] > word_lengths[longest_word_index]) {
                longest_word_index = i;
            }
            
            if (word_lengths[i] < word_lengths[shortest_word_index]) {
                shortest_word_index = i;
            }
        }
        
        double avg_length = (double)total_chars_in_words / word_count;
        
        printf("\nStatistics:\n");
        printf("Average word length: %.2f\n", avg_length);
        printf("Longest word: '%s' (%d characters)\n", words[longest_word_index], word_lengths[longest_word_index]);
        printf("Shortest word: '%s' (%d characters)\n", words[shortest_word_index], word_lengths[shortest_word_index]);
    }
    
    char reversed[MAX_INPUT_LEN + 1];
    if (input_len > 0) {
        for (size_t i = 0; i < input_len; i++) {
            reversed[i] = input[input_len - 1 - i];
        }
        reversed[input_len] = '\0';
        printf("\nReversed text: %s\n", reversed);
    }
    
    return 0;
}