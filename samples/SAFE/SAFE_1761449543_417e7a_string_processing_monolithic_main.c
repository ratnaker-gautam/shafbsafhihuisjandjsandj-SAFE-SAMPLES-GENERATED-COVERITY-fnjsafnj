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
        printf("Empty input\n");
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
        
        if (isalnum((unsigned char)c) || c == '\'' || c == '-') {
            if (word_pos < MAX_WORD_LEN) {
                current_word[word_pos++] = c;
                in_word = 1;
            }
        } else {
            if (in_word && word_pos > 0) {
                if (word_count < MAX_WORDS) {
                    current_word[word_pos] = '\0';
                    strncpy(words[word_count], current_word, MAX_WORD_LEN);
                    words[word_count][MAX_WORD_LEN] = '\0';
                    word_lengths[word_count] = word_pos;
                    word_count++;
                    char_count += word_pos;
                }
                word_pos = 0;
                in_word = 0;
            }
        }
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total characters: %zu\n", input_len);
    printf("Total words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (float)char_count / word_count);
        
        int longest_idx = 0;
        int shortest_idx = 0;
        
        for (int i = 1; i < word_count; i++) {
            if (word_lengths[i] > word_lengths[longest_idx]) {
                longest_idx = i;
            }
            if (word_lengths[i] < word_lengths[shortest_idx]) {
                shortest_idx = i;
            }
        }
        
        printf("Longest word: %s (%d characters)\n", words[longest_idx], word_lengths[longest_idx]);
        printf("Shortest word: %s (%d characters)\n", words[shortest_idx], word_lengths[shortest_idx]);
        
        printf("\nWord frequency analysis:\n");
        int processed[MAX_WORDS] = {0};
        
        for (int i = 0; i < word_count; i++) {
            if (!processed[i]) {
                int count = 1;
                for (int j = i + 1; j < word_count; j++) {
                    if (strcmp(words[i], words[j]) == 0) {
                        count++;
                        processed[j] = 1;
                    }
                }
                printf("'%s': appears %d time(s)\n", words[i], count);
                processed[i] = 1;
            }
        }
    }
    
    printf("\nProcessed text (uppercase): ");
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        if (isalpha((unsigned char)c)) {
            putchar(toupper((unsigned char)c));
        } else {
            putchar(c);
        }
    }
    printf("\n");
    
    return 0;
}