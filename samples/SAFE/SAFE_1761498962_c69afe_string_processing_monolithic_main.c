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
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int in_word = 0;
    int current_word_len = 0;
    char current_word[64];
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (isalnum((unsigned char)c) || c == '\'' || c == '-') {
            if (!in_word) {
                in_word = 1;
                current_word_len = 0;
            }
            
            if (current_word_len < 63) {
                current_word[current_word_len++] = c;
            }
        } else {
            if (in_word && current_word_len > 0) {
                if (word_count < MAX_WORDS) {
                    current_word[current_word_len] = '\0';
                    strncpy(words[word_count], current_word, 63);
                    words[word_count][63] = '\0';
                    word_lengths[word_count] = current_word_len;
                    total_chars += current_word_len;
                    word_count++;
                }
                in_word = 0;
                current_word_len = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nText analysis results:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters in words: %d\n", total_chars);
    printf("Average word length: %.2f\n", (float)total_chars / word_count);
    
    printf("\nWords found:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%d. %s (length: %d)\n", i + 1, words[i], word_lengths[i]);
    }
    
    char longest_word[64] = "";
    char shortest_word[64] = "";
    int max_len = 0;
    int min_len = 64;
    
    for (int i = 0; i < word_count; i++) {
        if (word_lengths[i] > max_len) {
            max_len = word_lengths[i];
            strncpy(longest_word, words[i], 63);
            longest_word[63] = '\0';
        }
        if (word_lengths[i] < min_len) {
            min_len = word_lengths[i];
            strncpy(shortest_word, words[i], 63);
            shortest_word[63] = '\0';
        }
    }
    
    printf("\nLongest word: %s (%d characters)\n", longest_word, max_len);
    printf("Shortest word: %s (%d characters)\n", shortest_word, min_len);
    
    int freq[26] = {0};
    for (int i = 0; i < word_count; i++) {
        for (int j = 0; words[i][j] != '\0'; j++) {
            char ch = tolower((unsigned char)words[i][j]);
            if (ch >= 'a' && ch <= 'z') {
                freq[ch - 'a']++;
            }
        }
    }
    
    printf("\nLetter frequency:\n");
    for (int i = 0; i < 26; i++) {
        if (freq[i] > 0) {
            printf("%c: %d ", 'a' + i, freq[i]);
        }
    }
    printf("\n");
    
    return 0;
}