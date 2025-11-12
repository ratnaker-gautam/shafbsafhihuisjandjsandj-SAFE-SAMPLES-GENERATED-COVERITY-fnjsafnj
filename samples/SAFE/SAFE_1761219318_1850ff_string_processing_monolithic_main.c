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
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char current_word[MAX_WORD_LEN + 1];
    int word_idx = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '_') {
            if (!in_word) {
                in_word = 1;
                word_idx = 0;
            }
            
            if (word_idx < MAX_WORD_LEN) {
                current_word[word_idx++] = c;
            }
        } else {
            if (in_word) {
                if (word_idx > 0) {
                    current_word[word_idx] = '\0';
                    
                    if (word_count < MAX_WORDS) {
                        strncpy(words[word_count], current_word, MAX_WORD_LEN);
                        words[word_count][MAX_WORD_LEN] = '\0';
                        word_lengths[word_count] = word_idx;
                        word_count++;
                        char_count += word_idx;
                    }
                }
                in_word = 0;
                word_idx = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nText analysis results:\n");
    printf("Total characters: %zu\n", input_len);
    printf("Total words: %d\n", word_count);
    printf("Average word length: %.2f\n", (float)char_count / word_count);
    
    printf("\nWords found:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%d. %s (length: %d)\n", i + 1, words[i], word_lengths[i]);
    }
    
    printf("\nWord length distribution:\n");
    int max_len = 0;
    for (int i = 0; i < word_count; i++) {
        if (word_lengths[i] > max_len) {
            max_len = word_lengths[i];
        }
    }
    
    for (int len = 1; len <= max_len; len++) {
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