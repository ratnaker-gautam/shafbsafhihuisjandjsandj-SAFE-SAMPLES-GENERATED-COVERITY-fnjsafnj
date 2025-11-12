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
    
    int in_word = 0;
    int current_word_len = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '\'' || c == '-') {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            
            if (current_word_len < MAX_WORD_LEN && word_count < MAX_WORDS) {
                words[word_count][current_word_len] = c;
                current_word_len++;
            }
        } else {
            if (in_word) {
                if (current_word_len > 0 && word_count < MAX_WORDS) {
                    words[word_count][current_word_len] = '\0';
                    word_lengths[word_count] = current_word_len;
                    word_count++;
                    char_count += current_word_len;
                }
                in_word = 0;
                current_word_len = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nText analysis:\n");
    printf("Total characters: %zu\n", input_len);
    printf("Total words: %d\n", word_count);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)char_count / word_count : 0.0);
    
    printf("\nWords found:\n");
    for (int i = 0; i < word_count; i++) {
        if (i < 10) {
            printf("%d. %s (length: %d)\n", i + 1, words[i], word_lengths[i]);
        } else {
            printf("%d. %s\n", i + 1, words[i]);
        }
    }
    
    if (word_count > 10) {
        printf("... and %d more words\n", word_count - 10);
    }
    
    char longest_word[MAX_WORD_LEN + 1] = "";
    char shortest_word[MAX_WORD_LEN + 1] = "";
    int max_len = 0;
    int min_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        int len = word_lengths[i];
        if (len > max_len) {
            max_len = len;
            strncpy(longest_word, words[i], MAX_WORD_LEN);
            longest_word[MAX_WORD_LEN] = '\0';
        }
        if (len < min_len && len > 0) {
            min_len = len;
            strncpy(shortest_word, words[i], MAX_WORD_LEN);
            shortest_word[MAX_WORD_LEN] = '\0';
        }
    }
    
    if (max_len > 0) {
        printf("\nLongest word: %s (%d characters)\n", longest_word, max_len);
    }
    if (min_len < MAX_WORD_LEN && min_len > 0) {
        printf("Shortest word: %s (%d characters)\n", shortest_word, min_len);
    }
    
    return 0;
}