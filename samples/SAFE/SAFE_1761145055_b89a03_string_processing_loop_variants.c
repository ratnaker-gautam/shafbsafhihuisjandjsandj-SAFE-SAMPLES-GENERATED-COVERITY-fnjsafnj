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
    while (i < len) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < 64 && word_count < MAX_WORDS) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int count = 0;
        char *word = words[j];
        size_t word_len = strlen(word);
        
        for (int k = 0; k < word_count; k++) {
            if (strcmp(word, words[k]) == 0) {
                count++;
            }
        }
        
        int duplicate = 0;
        for (int m = 0; m < j; m++) {
            if (strcmp(word, words[m]) == 0) {
                duplicate = 1;
                break;
            }
        }
        
        if (!duplicate) {
            printf("'%s' appears %d time(s)\n", word, count);
        }
    }
    
    char longest[64] = "";
    char shortest[64] = "";
    size_t min_len = 64;
    size_t max_len = 0;
    
    for (int n = 0; n < word_count; n++) {
        size_t current_len = strlen(words[n]);
        if (current_len > max_len) {
            max_len = current_len;
            strcpy(longest, words[n]);
        }
        if (current_len < min_len && current_len > 0) {
            min_len = current_len;
            strcpy(shortest, words[n]);
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: '%s' (%zu characters)\n", longest, max_len);
    }
    if (min_len < 64) {
        printf("Shortest word: '%s' (%zu characters)\n", shortest, min_len);
    }
    
    return 0;
}