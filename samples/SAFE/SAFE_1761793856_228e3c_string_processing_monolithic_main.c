//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int char_count = 0;
    int word_lengths[MAX_WORDS] = {0};
    
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
    
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '\'' || c == '-') {
            if (word_pos < MAX_WORD_LEN - 1) {
                current_word[word_pos++] = c;
                in_word = 1;
            }
        } else {
            if (in_word && word_pos > 0) {
                current_word[word_pos] = '\0';
                
                if (word_count < MAX_WORDS) {
                    strncpy(words[word_count], current_word, MAX_WORD_LEN - 1);
                    words[word_count][MAX_WORD_LEN - 1] = '\0';
                    word_lengths[word_count] = word_pos;
                    word_count++;
                }
                
                word_pos = 0;
                in_word = 0;
            }
        }
        
        if (c != '\0') {
            char_count++;
        }
    }
    
    printf("\nAnalysis Results:\n");
    printf("Total characters: %d\n", char_count);
    printf("Total words: %d\n", word_count);
    
    if (word_count > 0) {
        int total_word_length = 0;
        int shortest = word_lengths[0];
        int longest = word_lengths[0];
        
        for (int i = 0; i < word_count; i++) {
            total_word_length += word_lengths[i];
            if (word_lengths[i] < shortest) shortest = word_lengths[i];
            if (word_lengths[i] > longest) longest = word_lengths[i];
        }
        
        printf("Average word length: %.2f\n", (float)total_word_length / word_count);
        printf("Shortest word: %d characters\n", shortest);
        printf("Longest word: %d characters\n", longest);
        
        printf("\nWord list:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d. %s (%d chars)\n", i + 1, words[i], word_lengths[i]);
        }
    }
    
    return 0;
}