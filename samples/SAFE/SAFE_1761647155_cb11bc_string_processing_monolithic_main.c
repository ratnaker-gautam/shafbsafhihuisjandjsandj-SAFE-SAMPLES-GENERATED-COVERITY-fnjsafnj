//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int word_lengths[MAX_WORDS] = {0};
    
    printf("Enter text: ");
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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
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
            if (current_word_len < 63 && word_count < MAX_WORDS) {
                words[word_count][current_word_len] = tolower((unsigned char)input[i]);
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
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("Found %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        if (word_lengths[i] > 0) {
            printf("Word %d: %s (length: %d)\n", i + 1, words[i], word_lengths[i]);
        }
    }
    
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
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters in words: %d\n", total_chars);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Longest word length: %d\n", longest_len);
    printf("Average word length: %.2f\n", avg_length);
    
    int freq[64] = {0};
    for (int i = 0; i < word_count; i++) {
        if (word_lengths[i] > 0 && word_lengths[i] < 64) {
            freq[word_lengths[i]]++;
        }
    }
    
    printf("\nWord length frequency:\n");
    for (int i = 1; i < 64; i++) {
        if (freq[i] > 0) {
            printf("Length %d: %d words\n", i, freq[i]);
        }
    }
    
    return 0;
}