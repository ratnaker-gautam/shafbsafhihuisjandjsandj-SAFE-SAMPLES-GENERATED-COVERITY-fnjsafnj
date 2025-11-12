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
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], &input[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        char processed[64];
        
        while (words[j][k] != '\0' && k < 63) {
            processed[k] = tolower((unsigned char)words[j][k]);
            k++;
        }
        processed[k] = '\0';
        
        int is_palindrome = 1;
        int left = 0;
        int right = k - 1;
        
        while (left < right) {
            if (processed[left] != processed[right]) {
                is_palindrome = 0;
                break;
            }
            left++;
            right--;
        }
        
        printf("Word %d: '%s'", j + 1, words[j]);
        if (is_palindrome) {
            printf(" (palindrome)");
        }
        printf("\n");
    }
    
    char longest[64] = "";
    char shortest[64] = "";
    
    if (word_count > 0) {
        strncpy(longest, words[0], 63);
        strncpy(shortest, words[0], 63);
        longest[63] = '\0';
        shortest[63] = '\0';
        
        int idx = 1;
        while (idx < word_count) {
            size_t current_len = strlen(words[idx]);
            size_t longest_len = strlen(longest);
            size_t shortest_len = strlen(shortest);
            
            if (current_len > longest_len) {
                strncpy(longest, words[idx], 63);
                longest[63] = '\0';
            }
            
            if (current_len < shortest_len) {
                strncpy(shortest, words[idx], 63);
                shortest[63] = '\0';
            }
            idx++;
        }
        
        printf("Longest word: %s\n", longest);
        printf("Shortest word: %s\n", shortest);
    }
    
    return 0;
}