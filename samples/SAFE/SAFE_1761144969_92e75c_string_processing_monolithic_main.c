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
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN);
    
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
    
    char_count = input_len;
    line_count = 1;
    
    int in_word = 0;
    int word_start = 0;
    int current_word_len = 0;
    
    for (int i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
                current_word_len = 0;
            }
            current_word_len++;
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS && current_word_len <= MAX_WORD_LEN) {
                    if (current_word_len > 0) {
                        strncpy(words[word_count], &input[word_start], current_word_len);
                        words[word_count][current_word_len] = '\0';
                        word_count++;
                    }
                }
                in_word = 0;
            }
        }
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWords found:\n");
        for (int i = 0; i < word_count; i++) {
            if (i < MAX_WORDS) {
                printf("%d: %s\n", i + 1, words[i]);
            }
        }
        
        printf("\nWord lengths:\n");
        for (int i = 0; i < word_count; i++) {
            if (i < MAX_WORDS) {
                size_t len = strlen(words[i]);
                printf("%s: %zu characters\n", words[i], len);
            }
        }
        
        int total_word_chars = 0;
        for (int i = 0; i < word_count; i++) {
            if (i < MAX_WORDS) {
                total_word_chars += strlen(words[i]);
            }
        }
        
        if (word_count > 0) {
            double avg_length = (double)total_word_chars / word_count;
            printf("\nAverage word length: %.2f\n", avg_length);
        }
    }
    
    printf("\nCharacter frequency (letters only):\n");
    int letter_freq[26] = {0};
    int total_letters = 0;
    
    for (int i = 0; i < input_len; i++) {
        char c = input[i];
        if (isalpha((unsigned char)c)) {
            total_letters++;
            int index = tolower((unsigned char)c) - 'a';
            if (index >= 0 && index < 26) {
                letter_freq[index]++;
            }
        }
    }
    
    for (int i = 0; i < 26; i++) {
        if (letter_freq[i] > 0) {
            printf("%c: %d (%.1f%%)\n", 'a' + i, letter_freq[i], 
                   total_letters > 0 ? (100.0 * letter_freq[i] / total_letters) : 0.0);
        }
    }
    
    return 0;
}