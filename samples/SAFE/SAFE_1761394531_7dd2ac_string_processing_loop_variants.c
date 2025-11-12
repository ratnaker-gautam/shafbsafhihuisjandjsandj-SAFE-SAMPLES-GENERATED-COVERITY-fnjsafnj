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
        
        if (i >= len) {
            break;
        }
        
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
    
    printf("Words found: %d\n", word_count);
    
    for (int outer = 0; outer < word_count; outer++) {
        int processed = 0;
        for (int inner = 0; inner < outer; inner++) {
            if (strcmp(words[outer], words[inner]) == 0) {
                processed = 1;
                break;
            }
        }
        
        if (processed) {
            continue;
        }
        
        int count = 0;
        int j = 0;
        do {
            if (strcmp(words[outer], words[j]) == 0) {
                count++;
            }
            j++;
        } while (j < word_count);
        
        printf("'%s' appears %d time(s)\n", words[outer], count);
    }
    
    char longest[64] = "";
    char shortest[64] = "";
    strcpy(shortest, words[0]);
    
    int k = 0;
    while (k < word_count) {
        size_t current_len = strlen(words[k]);
        size_t longest_len = strlen(longest);
        size_t shortest_len = strlen(shortest);
        
        if (current_len > longest_len) {
            strncpy(longest, words[k], 63);
            longest[63] = '\0';
        }
        
        if (current_len < shortest_len) {
            strncpy(shortest, words[k], 63);
            shortest[63] = '\0';
        }
        k++;
    }
    
    printf("Longest word: '%s'\n", longest);
    printf("Shortest word: '%s'\n", shortest);
    
    return 0;
}