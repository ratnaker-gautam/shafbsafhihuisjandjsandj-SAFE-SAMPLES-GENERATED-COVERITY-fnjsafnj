//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN + 1];
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
                
                if (word_len >= MAX_INPUT_LEN) {
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
    
    printf("\nWord analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    int longest_word_index = 0;
    int shortest_word_index = 0;
    
    for (int i = 0; i < word_count; i++) {
        total_chars += word_lengths[i];
        
        if (word_lengths[i] > word_lengths[longest_word_index]) {
            longest_word_index = i;
        }
        
        if (word_lengths[i] < word_lengths[shortest_word_index]) {
            shortest_word_index = i;
        }
    }
    
    printf("Average word length: %.2f\n", (float)total_chars / word_count);
    printf("Longest word: '%s' (%d characters)\n", words[longest_word_index], word_lengths[longest_word_index]);
    printf("Shortest word: '%s' (%d characters)\n", words[shortest_word_index], word_lengths[shortest_word_index]);
    
    printf("\nWords in reverse order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s", words[i]);
        if (i > 0) {
            printf(" ");
        }
    }
    printf("\n");
    
    return 0;
}