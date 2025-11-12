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
    int word_lengths[MAX_WORDS] = {0};
    
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
    int current_word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            
            if (word_count < MAX_WORDS && current_word_len < MAX_WORD_LEN) {
                words[word_count][current_word_len] = tolower((unsigned char)input[i]);
                current_word_len++;
            }
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS && current_word_len > 0) {
                    words[word_count][current_word_len] = '\0';
                    word_lengths[word_count] = current_word_len;
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    printf("\nProcessing results:\n");
    printf("Total characters: %zu\n", input_len);
    printf("Words found: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWords (lowercase):\n");
        for (int i = 0; i < word_count; i++) {
            if (i < 10) {
                printf("%d: %s (length: %d)\n", i + 1, words[i], word_lengths[i]);
            }
        }
        
        if (word_count > 10) {
            printf("... and %d more words\n", word_count - 10);
        }
        
        int total_letters = 0;
        int longest_word = 0;
        int shortest_word = MAX_WORD_LEN;
        
        for (int i = 0; i < word_count; i++) {
            total_letters += word_lengths[i];
            if (word_lengths[i] > longest_word) {
                longest_word = word_lengths[i];
            }
            if (word_lengths[i] < shortest_word) {
                shortest_word = word_lengths[i];
            }
        }
        
        printf("\nWord statistics:\n");
        printf("Average word length: %.2f\n", word_count > 0 ? (float)total_letters / word_count : 0.0f);
        printf("Longest word: %d letters\n", longest_word);
        printf("Shortest word: %d letters\n", shortest_word);
        
        printf("\nWord frequency by length:\n");
        int length_freq[MAX_WORD_LEN + 1] = {0};
        for (int i = 0; i < word_count; i++) {
            if (word_lengths[i] <= MAX_WORD_LEN) {
                length_freq[word_lengths[i]]++;
            }
        }
        
        for (int i = 1; i <= MAX_WORD_LEN; i++) {
            if (length_freq[i] > 0) {
                printf("%d-letter words: %d\n", i, length_freq[i]);
            }
        }
    }
    
    return 0;
}