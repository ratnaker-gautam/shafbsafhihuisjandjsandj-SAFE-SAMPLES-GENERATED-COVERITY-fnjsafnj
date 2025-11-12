//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int word_length = 0;
    int in_word = 0;
    int i;
    
    printf("Enter text to analyze: ");
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
    
    for (i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_length = 0;
            }
            
            if (word_count < MAX_WORDS && word_length < 63) {
                words[word_count][word_length++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS) {
                    words[word_count][word_length] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nWord frequency analysis:\n");
    printf("Total unique words: ");
    
    int unique_count = 0;
    int processed[MAX_WORDS] = {0};
    
    for (i = 0; i < word_count; i++) {
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
        
        unique_count++;
        printf("%s: %d\n", words[i], count);
    }
    
    printf("%d\n", unique_count);
    
    char longest_word[64] = "";
    char shortest_word[64] = "";
    int max_len = 0;
    int min_len = 64;
    
    for (i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
            strcpy(longest_word, words[i]);
        }
        if (len < min_len) {
            min_len = len;
            strcpy(shortest_word, words[i]);
        }
    }
    
    printf("Longest word: %s (%d letters)\n", longest_word, max_len);
    printf("Shortest word: %s (%d letters)\n", shortest_word, min_len);
    
    int total_letters = 0;
    for (i = 0; i < word_count; i++) {
        total_letters += strlen(words[i]);
    }
    
    double avg_length = (double)total_letters / word_count;
    printf("Average word length: %.2f\n", avg_length);
    
    return 0;
}