//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_INPUT + 1];
    int word_count = 0;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT);
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
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > MAX_INPUT) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        for (int j = 0; j < word_len; j++) {
            words[word_count][j] = input[start + j];
        }
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    printf("Found %d words:\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        char reversed[MAX_INPUT + 1];
        int word_len = strlen(words[idx]);
        
        if (word_len > MAX_INPUT) {
            fprintf(stderr, "Word length error\n");
            return 1;
        }
        
        for (int pos = 0; pos < word_len; pos++) {
            reversed[word_len - pos - 1] = words[idx][pos];
        }
        reversed[word_len] = '\0';
        
        int vowel_count = 0;
        char current_char;
        for (int k = 0; k < word_len; k++) {
            current_char = tolower((unsigned char)words[idx][k]);
            if (current_char == 'a' || current_char == 'e' || 
                current_char == 'i' || current_char == 'o' || 
                current_char == 'u') {
                vowel_count++;
            }
        }
        
        printf("Word %d: '%s' (reversed: '%s', vowels: %d)\n", 
               idx + 1, words[idx], reversed, vowel_count);
    }
    
    char longest[MAX_INPUT + 1] = "";
    char shortest[MAX_INPUT + 1] = "";
    int longest_len = 0;
    int shortest_len = MAX_INPUT + 1;
    
    for (int idx = 0; idx < word_count; idx++) {
        int current_len = strlen(words[idx]);
        
        if (current_len > longest_len) {
            longest_len = current_len;
            strncpy(longest, words[idx], MAX_INPUT);
            longest[MAX_INPUT] = '\0';
        }
        
        if (current_len < shortest_len && current_len > 0) {
            shortest_len = current_len;
            strncpy(shortest, words[idx], MAX_INPUT);
            shortest[MAX_INPUT] = '\0';
        }
    }
    
    if (word_count > 0) {
        printf("Longest word: '%s' (%d characters)\n", longest, longest_len);
        printf("Shortest word: '%s' (%d characters)\n", shortest, shortest_len);
    }
    
    return 0;
}