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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int current_word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            
            if (current_word_len < MAX_WORD_LEN) {
                if (word_count < MAX_WORDS) {
                    words[word_count][current_word_len] = input[i];
                    current_word_len++;
                }
            }
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS && current_word_len > 0) {
                    words[word_count][current_word_len] = '\0';
                    word_lengths[word_count] = current_word_len;
                    word_count++;
                }
                in_word = 0;
                current_word_len = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nWord analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int total_length = 0;
    int shortest_len = MAX_WORD_LEN + 1;
    int longest_len = 0;
    
    for (int i = 0; i < word_count; i++) {
        total_length += word_lengths[i];
        
        if (word_lengths[i] < shortest_len) {
            shortest_len = word_lengths[i];
        }
        
        if (word_lengths[i] > longest_len) {
            longest_len = word_lengths[i];
        }
    }
    
    double avg_length = (double)total_length / word_count;
    printf("Average word length: %.2f\n", avg_length);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Longest word length: %d\n", longest_len);
    
    printf("\nWords sorted by length:\n");
    
    for (int len = shortest_len; len <= longest_len; len++) {
        for (int i = 0; i < word_count; i++) {
            if (word_lengths[i] == len) {
                printf("%s (length: %d)\n", words[i], len);
            }
        }
    }
    
    printf("\nWord frequency by length:\n");
    for (int len = shortest_len; len <= longest_len; len++) {
        int count = 0;
        for (int i = 0; i < word_count; i++) {
            if (word_lengths[i] == len) {
                count++;
            }
        }
        if (count > 0) {
            printf("Length %d: %d words\n", len, count);
        }
    }
    
    return 0;
}