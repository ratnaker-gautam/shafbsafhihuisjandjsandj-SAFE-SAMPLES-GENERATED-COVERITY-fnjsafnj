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
    
    printf("Words found: %d\n", word_count);
    printf("Processing words...\n");
    
    for (int j = 0; j < word_count; j++) {
        char processed[64];
        int k = 0;
        int l = 0;
        
        while (words[j][k] != '\0' && l < 63) {
            if (isalpha(words[j][k])) {
                processed[l] = tolower(words[j][k]);
                l++;
            }
            k++;
        }
        processed[l] = '\0';
        
        if (l > 0) {
            printf("Word %d: '%s' -> '%s'\n", j+1, words[j], processed);
        } else {
            printf("Word %d: '%s' -> (empty)\n", j+1, words[j]);
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
    
    int unique_count = 0;
    for (int j = 0; j < word_count; j++) {
        int is_unique = 1;
        for (int k = 0; k < j; k++) {
            if (strcmp(words[j], words[k]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            unique_count++;
        }
    }
    
    printf("Unique words: %d\n", unique_count);
    
    return 0;
}