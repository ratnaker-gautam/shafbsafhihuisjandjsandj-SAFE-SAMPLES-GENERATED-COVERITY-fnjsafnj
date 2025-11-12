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
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= 64) {
            word_len = 63;
        }
        
        strncpy(words[word_count], &input[start], word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Processing %d words:\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        char processed[64];
        size_t k = 0;
        size_t m = 0;
        
        while (words[j][k] != '\0' && m < 63) {
            if (isalpha(words[j][k])) {
                processed[m] = tolower(words[j][k]);
                m++;
            }
            k++;
        }
        processed[m] = '\0';
        
        if (m > 0) {
            printf("Original: %s -> Processed: %s\n", words[j], processed);
        } else {
            printf("Original: %s -> No alphabetic characters\n", words[j]);
        }
    }
    
    int total_chars = 0;
    int idx = 0;
    do {
        if (idx < word_count) {
            total_chars += strlen(words[idx]);
        }
        idx++;
    } while (idx < word_count);
    
    printf("Total characters in all words: %d\n", total_chars);
    
    char longest[64] = "";
    int longest_len = 0;
    
    for (int j = 0; j < word_count; j++) {
        int current_len = strlen(words[j]);
        if (current_len > longest_len) {
            longest_len = current_len;
            strncpy(longest, words[j], 63);
            longest[63] = '\0';
        }
    }
    
    if (longest_len > 0) {
        printf("Longest word: %s (%d characters)\n", longest, longest_len);
    }
    
    return 0;
}