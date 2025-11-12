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
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
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
            words[word_count][k] = tolower(input[i + k]);
        }
        words[word_count][word_len] = '\0';
        word_count++;
        
        i = j;
    }
    
    printf("Processed words:\n");
    for (i = 0; i < word_count; i++) {
        int unique = 1;
        
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                unique = 0;
                break;
            }
        }
        
        if (unique) {
            int count = 0;
            for (k = 0; k < word_count; k++) {
                if (strcmp(words[i], words[k]) == 0) {
                    count++;
                }
            }
            printf("'%s': %d occurrence(s)\n", words[i], count);
        }
    }
    
    char longest[64] = "";
    char shortest[64] = "";
    
    if (word_count > 0) {
        strncpy(longest, words[0], 63);
        strncpy(shortest, words[0], 63);
        longest[63] = '\0';
        shortest[63] = '\0';
        
        for (i = 1; i < word_count; i++) {
            if (strlen(words[i]) > strlen(longest)) {
                strncpy(longest, words[i], 63);
                longest[63] = '\0';
            }
            if (strlen(words[i]) < strlen(shortest)) {
                strncpy(shortest, words[i], 63);
                shortest[63] = '\0';
            }
        }
        
        printf("Longest word: %s\n", longest);
        printf("Shortest word: %s\n", shortest);
    }
    
    return 0;
}