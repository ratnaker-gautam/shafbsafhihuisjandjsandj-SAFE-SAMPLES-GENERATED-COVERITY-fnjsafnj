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
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Processing %d words:\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        int k = 0;
        char processed[64];
        int has_alpha = 0;
        
        while (words[j][k] != '\0' && k < 63) {
            if (isalpha((unsigned char)words[j][k])) {
                processed[k] = toupper((unsigned char)words[j][k]);
                has_alpha = 1;
            } else {
                processed[k] = words[j][k];
            }
            k++;
        }
        processed[k] = '\0';
        
        if (has_alpha) {
            printf("Word %d: %s -> %s\n", j+1, words[j], processed);
        } else {
            printf("Word %d: %s (no letters)\n", j+1, words[j]);
        }
    }
    
    int total_chars = 0;
    int j = 0;
    do {
        if (j < word_count) {
            total_chars += strlen(words[j]);
        }
        j++;
    } while (j < word_count);
    
    printf("Total characters in all words: %d\n", total_chars);
    
    char longest[64] = "";
    for (int j = 0; j < word_count; j++) {
        if (strlen(words[j]) > strlen(longest)) {
            strncpy(longest, words[j], 63);
            longest[63] = '\0';
        }
    }
    
    printf("Longest word: %s\n", longest);
    
    return 0;
}