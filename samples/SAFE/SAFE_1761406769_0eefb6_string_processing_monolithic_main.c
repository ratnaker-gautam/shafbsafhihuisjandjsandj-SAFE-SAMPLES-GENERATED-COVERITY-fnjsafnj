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
    
    char *ptr = input;
    int in_word = 0;
    int current_word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (isalnum((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            current_word_len++;
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS && current_word_len < 64) {
                    size_t copy_len = (size_t)current_word_len;
                    if (copy_len > 63) copy_len = 63;
                    memcpy(words[word_count], &input[i - current_word_len], copy_len);
                    words[word_count][copy_len] = '\0';
                    word_lengths[word_count] = current_word_len;
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nWord Analysis Results:\n");
    printf("Total words: %d\n", word_count);
    
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
    
    printf("Average word length: %.2f\n", (float)total_chars / word_count);
    printf("Longest word length: %d\n", longest_len);
    printf("Shortest word length: %d\n", shortest_len);
    
    printf("\nWord frequency by length:\n");
    int freq[65] = {0};
    for (int i = 0; i < word_count; i++) {
        if (word_lengths[i] >= 0 && word_lengths[i] < 65) {
            freq[word_lengths[i]]++;
        }
    }
    
    for (int i = 1; i < 65; i++) {
        if (freq[i] > 0) {
            printf("Length %d: %d words\n", i, freq[i]);
        }
    }
    
    printf("\nWords sorted by length:\n");
    for (int len = shortest_len; len <= longest_len; len++) {
        for (int i = 0; i < word_count; i++) {
            if (word_lengths[i] == len) {
                printf("%s (length %d)\n", words[i], len);
            }
        }
    }
    
    return 0;
}