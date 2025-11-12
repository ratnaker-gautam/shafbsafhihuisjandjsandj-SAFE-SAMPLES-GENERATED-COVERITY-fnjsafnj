//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 256

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
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        printf("Word %d: ", j + 1);
        
        int k = 0;
        do {
            if (k < strlen(words[j])) {
                putchar(words[j][k]);
            }
            k++;
        } while (k <= strlen(words[j]));
        printf("\n");
    }
    
    char search[64];
    printf("Enter word to search: ");
    if (scanf("%63s", search) != 1) {
        fprintf(stderr, "Error reading search word\n");
        return 1;
    }
    
    int found = 0;
    int idx = 0;
    while (idx < word_count) {
        if (strcmp(words[idx], search) == 0) {
            found = 1;
            break;
        }
        idx++;
    }
    
    if (found) {
        printf("Word '%s' found at position %d\n", search, idx + 1);
    } else {
        printf("Word '%s' not found\n", search);
    }
    
    int total_chars = 0;
    for (int m = 0; m < word_count; m++) {
        int n = 0;
        while (words[m][n] != '\0') {
            total_chars++;
            n++;
        }
    }
    
    printf("Total characters in all words: %d\n", total_chars);
    
    return 0;
}