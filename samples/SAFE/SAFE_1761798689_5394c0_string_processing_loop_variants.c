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
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
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
            if (tolower((unsigned char)words[i][j]) != 
                tolower((unsigned char)words[i][word_len - 1 - j])) {
                is_palindrome = 0;
                break;
            }
        }
        
        printf("Word %d: \"%s\" (length: %zu)", i + 1, words[i], word_len);
        if (is_palindrome && word_len > 0) {
            printf(" [palindrome]");
        }
        printf("\n");
    }
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
    }
    
    printf("Total characters in words: %d\n", total_chars);
    
    char longest_word[64] = "";
    size_t max_len = 0;
    for (i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        if (current_len > max_len) {
            max_len = current_len;
            strncpy(longest_word, words[i], sizeof(longest_word) - 1);
            longest_word[sizeof(longest_word) - 1] = '\0';
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: \"%s\" (length: %zu)\n", longest_word, max_len);
    }
    
    return 0;
}