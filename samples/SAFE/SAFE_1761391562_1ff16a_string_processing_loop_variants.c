//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text: ");
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
    
    size_t i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        size_t word_start = i;
        while (i < input_len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        size_t word_len = i - word_start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        for (size_t j = 0; j < word_len; j++) {
            words[word_count][j] = input[word_start + j];
        }
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        size_t len = strlen(words[idx]);
        for (size_t pos = 0; pos < len; pos++) {
            if (islower((unsigned char)words[idx][pos])) {
                words[idx][pos] = toupper((unsigned char)words[idx][pos]);
            }
        }
    }
    
    printf("Processed words:\n");
    int counter = 0;
    do {
        if (counter < word_count) {
            printf("%s\n", words[counter]);
        }
        counter++;
    } while (counter < word_count);
    
    char longest_word[MAX_WORD_LEN] = "";
    size_t max_len = 0;
    
    for (int idx = 0; idx < word_count; idx++) {
        size_t current_len = strlen(words[idx]);
        if (current_len > max_len) {
            max_len = current_len;
            strncpy(longest_word, words[idx], MAX_WORD_LEN - 1);
            longest_word[MAX_WORD_LEN - 1] = '\0';
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: %s (length: %zu)\n", longest_word, max_len);
    }
    
    return 0;
}