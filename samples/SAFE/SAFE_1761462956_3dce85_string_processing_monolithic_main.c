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
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < 64) {
            strncpy(words[word_count], token, 63);
            words[word_count][63] = '\0';
            word_lengths[word_count] = (int)token_len;
            total_chars += (int)token_len;
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nWord Analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters (words only): %d\n", total_chars);
    printf("Average word length: %.2f\n", (double)total_chars / word_count);
    
    printf("\nWords sorted by length:\n");
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (word_lengths[i] > word_lengths[j]) {
                int temp_len = word_lengths[i];
                word_lengths[i] = word_lengths[j];
                word_lengths[j] = temp_len;
                
                char temp_word[64];
                strncpy(temp_word, words[i], 63);
                temp_word[63] = '\0';
                strncpy(words[i], words[j], 63);
                words[i][63] = '\0';
                strncpy(words[j], temp_word, 63);
                words[j][63] = '\0';
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        printf("%d. %s (%d characters)\n", i + 1, words[i], word_lengths[i]);
    }
    
    printf("\nWord frequency by length:\n");
    int max_length = 0;
    for (int i = 0; i < word_count; i++) {
        if (word_lengths[i] > max_length) {
            max_length = word_lengths[i];
        }
    }
    
    if (max_length > 0) {
        for (int len = 1; len <= max_length; len++) {
            int count = 0;
            for (int i = 0; i < word_count; i++) {
                if (word_lengths[i] == len) {
                    count++;
                }
            }
            if (count > 0) {
                printf("%d-letter words: %d\n", len, count);
            }
        }
    }
    
    return 0;
}