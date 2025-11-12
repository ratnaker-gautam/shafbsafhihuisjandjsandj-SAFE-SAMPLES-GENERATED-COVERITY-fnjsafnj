//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    for (k = 0; k < word_count; k++) {
        printf("Word %d: %s\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    for (k = 0; k < word_count; k++) {
        char temp[MAX_WORD_LEN + 1];
        size_t word_len = strlen(words[k]);
        
        if (word_len == 0) {
            continue;
        }
        
        for (i = 0; i < word_len; i++) {
            temp[i] = words[k][word_len - 1 - i];
        }
        temp[word_len] = '\0';
        
        int is_palindrome = 1;
        for (i = 0; i < word_len / 2; i++) {
            if (tolower(temp[i]) != tolower(words[k][i])) {
                is_palindrome = 0;
                break;
            }
        }
        
        printf("Word '%s' reversed: '%s' %s\n", 
               words[k], temp, is_palindrome ? "(palindrome)" : "");
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int shortest_len = MAX_WORD_LEN;
    int longest_len = 0;
    
    k = 0;
    while (k < word_count) {
        int current_len = strlen(words[k]);
        total_chars += current_len;
        
        if (current_len < shortest_len) {
            shortest_len = current_len;
        }
        if (current_len > longest_len) {
            longest_len = current_len;
        }
        k++;
    }
    
    if (word_count > 0) {
        printf("Total characters in words: %d\n", total_chars);
        printf("Shortest word length: %d\n", shortest_len);
        printf("Longest word length: %d\n", longest_len);
        printf("Average word length: %.2f\n", (float)total_chars / word_count);
    }
    
    return 0;
}