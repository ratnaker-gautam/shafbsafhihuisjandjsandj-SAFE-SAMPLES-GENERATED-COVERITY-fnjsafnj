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
        return 0;
    }
    
    int i = 0;
    while (i < len) {
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
        if (word_len > 0 && word_len < 64 && word_count < MAX_WORDS) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int count = 1;
        for (int k = j + 1; k < word_count; k++) {
            if (strcmp(words[j], words[k]) == 0) {
                count++;
                for (int m = k; m < word_count - 1; m++) {
                    strcpy(words[m], words[m + 1]);
                }
                word_count--;
                k--;
            }
        }
        printf("'%s': %d\n", words[j], count);
    }
    
    char longest[64] = "";
    char shortest[64] = "";
    
    if (word_count > 0) {
        strcpy(longest, words[0]);
        strcpy(shortest, words[0]);
        
        int idx = 0;
        do {
            size_t current_len = strlen(words[idx]);
            if (current_len > strlen(longest)) {
                strcpy(longest, words[idx]);
            }
            if (current_len < strlen(shortest)) {
                strcpy(shortest, words[idx]);
            }
            idx++;
        } while (idx < word_count);
        
        printf("Longest word: '%s' (%zu characters)\n", longest, strlen(longest));
        printf("Shortest word: '%s' (%zu characters)\n", shortest, strlen(shortest));
    }
    
    return 0;
}