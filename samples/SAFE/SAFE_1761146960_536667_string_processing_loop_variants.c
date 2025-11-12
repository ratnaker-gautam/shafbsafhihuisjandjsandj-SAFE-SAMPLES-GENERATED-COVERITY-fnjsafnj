//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a string: ");
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Original words:\n");
    for (k = 0; k < word_count; k++) {
        printf("%s\n", words[k]);
    }
    
    printf("\nProcessing words:\n");
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        if (word_len == 0) {
            continue;
        }
        
        char processed[MAX_WORD_LEN + 1];
        size_t p_idx = 0;
        
        for (i = 0; i < word_len && p_idx < MAX_WORD_LEN; i++) {
            char c = words[k][i];
            if (isalpha((unsigned char)c)) {
                processed[p_idx] = tolower((unsigned char)c);
                p_idx++;
            }
        }
        processed[p_idx] = '\0';
        
        if (p_idx > 0) {
            printf("'%s' -> '%s' (length: %zu)\n", words[k], processed, p_idx);
        } else {
            printf("'%s' -> [empty]\n", words[k]);
        }
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int alpha_chars = 0;
    
    k = 0;
    while (k < word_count) {
        size_t word_len = strlen(words[k]);
        total_chars += word_len;
        
        for (i = 0; i < word_len; i++) {
            if (isalpha((unsigned char)words[k][i])) {
                alpha_chars++;
            }
        }
        k++;
    }
    
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", total_chars);
    printf("Alphabetic characters: %d\n", alpha_chars);
    
    return 0;
}