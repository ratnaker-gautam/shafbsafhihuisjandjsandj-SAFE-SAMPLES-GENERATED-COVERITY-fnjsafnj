//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = i;
        while (j < len && !isspace(input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len >= 64) {
            word_len = 63;
        }
        
        for (k = 0; k < word_len; k++) {
            words[word_count][k] = input[i + k];
        }
        words[word_count][word_len] = '\0';
        word_count++;
        
        i = j;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        int is_palindrome = 1;
        size_t word_len = strlen(words[i]);
        
        for (j = 0; j < word_len / 2; j++) {
            if (tolower(words[i][j]) != tolower(words[i][word_len - 1 - j])) {
                is_palindrome = 0;
                break;
            }
        }
        
        printf("Word %d: \"%s\" (length: %zu, palindrome: %s)\n", 
               i + 1, words[i], word_len, is_palindrome ? "yes" : "no");
    }
    
    char longest_word[64] = "";
    char shortest_word[64] = "";
    size_t max_len = 0;
    size_t min_len = 64;
    
    for (i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        
        if (current_len > max_len) {
            max_len = current_len;
            strncpy(longest_word, words[i], 63);
            longest_word[63] = '\0';
        }
        
        if (current_len < min_len && current_len > 0) {
            min_len = current_len;
            strncpy(shortest_word, words[i], 63);
            shortest_word[63] = '\0';
        }
    }
    
    if (word_count > 0) {
        printf("Longest word: \"%s\" (%zu characters)\n", longest_word, max_len);
        printf("Shortest word: \"%s\" (%zu characters)\n", shortest_word, min_len);
    }
    
    int letter_count[26] = {0};
    int total_letters = 0;
    
    for (i = 0; i < word_count; i++) {
        for (j = 0; words[i][j] != '\0'; j++) {
            char c = tolower(words[i][j]);
            if (c >= 'a' && c <= 'z') {
                letter_count[c - 'a']++;
                total_letters++;
            }
        }
    }
    
    if (total_letters > 0) {
        printf("Letter frequency:\n");
        for (i = 0; i < 26; i++) {
            if (letter_count[i] > 0) {
                double frequency = (double)letter_count[i] / total_letters * 100.0;
                printf("%c: %d (%.2f%%)\n", 'a' + i, letter_count[i], frequency);
            }
        }
    }
    
    return 0;
}