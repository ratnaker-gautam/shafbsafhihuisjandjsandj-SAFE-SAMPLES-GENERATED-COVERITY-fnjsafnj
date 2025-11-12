//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_INPUT_LEN / 2][MAX_WORD_LEN + 1];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int word_start = 0;
    
    for (size_t i = 0; i <= len; i++) {
        if (i < len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                int word_len = i - word_start;
                if (word_len > MAX_WORD_LEN) {
                    fprintf(stderr, "Word too long\n");
                    return 1;
                }
                
                if (word_count >= MAX_INPUT_LEN / 2) {
                    fprintf(stderr, "Too many words\n");
                    return 1;
                }
                
                strncpy(words[word_count], &input[word_start], word_len);
                words[word_count][word_len] = '\0';
                word_count++;
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nFound %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words...\n");
    
    int total_chars = 0;
    int longest_len = 0;
    int shortest_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        int current_len = strlen(words[i]);
        total_chars += current_len;
        
        if (current_len > longest_len) {
            longest_len = current_len;
        }
        
        if (current_len < shortest_len) {
            shortest_len = current_len;
        }
        
        for (int j = 0; j < current_len; j++) {
            words[i][j] = toupper((unsigned char)words[i][j]);
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total characters in words: %d\n", total_chars);
    printf("Longest word length: %d\n", longest_len);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0);
    
    printf("\nWords in uppercase:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}