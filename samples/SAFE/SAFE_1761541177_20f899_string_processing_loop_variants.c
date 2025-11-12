//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int word_lengths[MAX_WORDS];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        int j = 0;
        int start = i;
        while (i < input_len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_lengths[word_count] = j;
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Found %d words:\n", word_count);
    for (int idx = 0; idx < word_count; idx++) {
        printf("Word %d: '%s' (length: %d)\n", idx + 1, words[idx], word_lengths[idx]);
    }
    
    int total_chars = 0;
    int k = 0;
    do {
        total_chars += word_lengths[k];
        k++;
    } while (k < word_count);
    
    printf("Total characters in all words: %d\n", total_chars);
    
    char longest_word[MAX_WORD_LEN + 1];
    char shortest_word[MAX_WORD_LEN + 1];
    int longest_len = 0;
    int shortest_len = MAX_WORD_LEN + 1;
    
    for (int idx = 0; idx < word_count; idx++) {
        int len = word_lengths[idx];
        if (len > longest_len) {
            longest_len = len;
            strncpy(longest_word, words[idx], MAX_WORD_LEN);
            longest_word[MAX_WORD_LEN] = '\0';
        }
        if (len < shortest_len) {
            shortest_len = len;
            strncpy(shortest_word, words[idx], MAX_WORD_LEN);
            shortest_word[MAX_WORD_LEN] = '\0';
        }
    }
    
    printf("Longest word: '%s' (%d characters)\n", longest_word, longest_len);
    printf("Shortest word: '%s' (%d characters)\n", shortest_word, shortest_len);
    
    int palindrome_count = 0;
    for (int idx = 0; idx < word_count; idx++) {
        int is_palindrome = 1;
        int len = word_lengths[idx];
        int left = 0;
        int right = len - 1;
        
        while (left < right) {
            if (tolower((unsigned char)words[idx][left]) != tolower((unsigned char)words[idx][right])) {
                is_palindrome = 0;
                break;
            }
            left++;
            right--;
        }
        
        if (is_palindrome && len > 0) {
            palindrome_count++;
        }
    }
    
    printf("Number of palindromes: %d\n", palindrome_count);
    
    return 0;
}