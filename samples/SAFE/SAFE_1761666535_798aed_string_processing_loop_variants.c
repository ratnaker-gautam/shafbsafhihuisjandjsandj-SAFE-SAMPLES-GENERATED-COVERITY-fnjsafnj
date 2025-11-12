//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_INPUT + 1];
    int word_count = 0;
    
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
        return 0;
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
        if (word_len > 0 && word_len <= MAX_INPUT) {
            strncpy(words[word_count], &input[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int j = 0; j < word_count; j++) {
        char processed[MAX_INPUT + 1];
        int k = 0;
        int l = 0;
        
        while (words[j][k] != '\0' && l < MAX_INPUT) {
            if (isalpha((unsigned char)words[j][k])) {
                processed[l] = tolower((unsigned char)words[j][k]);
                l++;
            }
            k++;
        }
        processed[l] = '\0';
        
        if (l > 0) {
            printf("Word %d: '%s' -> '%s'\n", j + 1, words[j], processed);
        } else {
            printf("Word %d: '%s' -> (no letters)\n", j + 1, words[j]);
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
    
    printf("Total characters in words: %d\n", total_chars);
    
    return 0;
}