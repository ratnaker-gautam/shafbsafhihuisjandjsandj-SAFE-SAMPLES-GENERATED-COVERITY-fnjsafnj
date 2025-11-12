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
    int char_count = 0;
    int line_count = 0;
    
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
        printf("Empty input provided\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = len;
    line_count = 1;
    
    int in_word = 0;
    int word_start = 0;
    int current_word_len = 0;
    
    for (int i = 0; i <= len; i++) {
        char c = input[i];
        
        if (isalpha((unsigned char)c) && !in_word) {
            in_word = 1;
            word_start = i;
            current_word_len = 0;
        }
        
        if (in_word) {
            if (isalpha((unsigned char)c)) {
                current_word_len++;
                if (current_word_len >= 63) {
                    in_word = 0;
                    if (word_count < MAX_WORDS) {
                        strncpy(words[word_count], &input[word_start], 63);
                        words[word_count][63] = '\0';
                        word_count++;
                    }
                }
            } else {
                in_word = 0;
                if (word_count < MAX_WORDS && current_word_len > 0) {
                    strncpy(words[word_count], &input[word_start], current_word_len);
                    words[word_count][current_word_len] = '\0';
                    word_count++;
                }
            }
        }
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord list:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord statistics:\n");
        int total_word_len = 0;
        int shortest_len = 64;
        int longest_len = 0;
        
        for (int i = 0; i < word_count; i++) {
            int wlen = strlen(words[i]);
            total_word_len += wlen;
            
            if (wlen < shortest_len) {
                shortest_len = wlen;
            }
            if (wlen > longest_len) {
                longest_len = wlen;
            }
        }
        
        double avg_len = word_count > 0 ? (double)total_word_len / word_count : 0.0;
        printf("Shortest word: %d characters\n", shortest_len);
        printf("Longest word: %d characters\n", longest_len);
        printf("Average word length: %.2f characters\n", avg_len);
    }
    
    return 0;
}