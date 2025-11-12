//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: string_processing
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
    int char_count = 0;
    int word_length = 0;
    int in_word = 0;
    
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
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '_') {
            if (!in_word) {
                in_word = 1;
                word_length = 0;
            }
            
            if (word_length < 63) {
                if (word_count < MAX_WORDS) {
                    words[word_count][word_length++] = c;
                }
            }
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS && word_length > 0) {
                    words[word_count][word_length] = '\0';
                    word_count++;
                }
                in_word = 0;
                word_length = 0;
            }
        }
        
        if (c != '\0') {
            char_count++;
        }
    }
    
    printf("\nInput analysis:\n");
    printf("Total characters: %d\n", char_count);
    printf("Words found: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWords:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord statistics:\n");
        int total_word_len = 0;
        int max_len = 0;
        int min_len = 1000;
        
        for (int i = 0; i < word_count; i++) {
            int len = (int)strlen(words[i]);
            total_word_len += len;
            if (len > max_len) max_len = len;
            if (len < min_len) min_len = len;
        }
        
        printf("Average word length: %.2f\n", word_count > 0 ? (double)total_word_len / word_count : 0.0);
        printf("Longest word: %d characters\n", max_len);
        printf("Shortest word: %d characters\n", min_len);
    }
    
    printf("\nCharacter frequency:\n");
    int freq[256] = {0};
    
    for (size_t i = 0; i < input_len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (c < 256) {
            freq[c]++;
        }
    }
    
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0 && isprint(i)) {
            printf("'%c': %d\n", i, freq[i]);
        }
    }
    
    return 0;
}