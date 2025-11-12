//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 256
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORD_COUNT][MAX_WORD_LEN];
    int word_count = 0;
    int word_length = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (word_length < MAX_WORD_LEN - 1) {
                words[word_count][word_length++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (word_length > 0) {
                words[word_count][word_length] = '\0';
                word_count++;
                word_length = 0;
                
                if (word_count >= MAX_WORD_COUNT) {
                    break;
                }
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nWord frequency analysis:\n");
    printf("=======================\n");
    
    int processed[MAX_WORD_COUNT] = {0};
    int total_unique = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) {
            continue;
        }
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (!processed[j] && strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("%-20s: %d occurrence(s)\n", words[i], count);
        total_unique++;
        processed[i] = 1;
    }
    
    printf("\nTotal words: %d\n", word_count);
    printf("Unique words: %d\n", total_unique);
    
    if (total_unique > 0) {
        char longest_word[MAX_WORD_LEN] = "";
        char shortest_word[MAX_WORD_LEN] = "";
        size_t min_len = MAX_WORD_LEN;
        size_t max_len = 0;
        
        for (int i = 0; i < word_count; i++) {
            size_t len = strlen(words[i]);
            if (len > max_len) {
                max_len = len;
                strcpy(longest_word, words[i]);
            }
            if (len < min_len) {
                min_len = len;
                strcpy(shortest_word, words[i]);
            }
        }
        
        printf("Longest word: %s (%zu letters)\n", longest_word, max_len);
        printf("Shortest word: %s (%zu letters)\n", shortest_word, min_len);
    }
    
    return 0;
}