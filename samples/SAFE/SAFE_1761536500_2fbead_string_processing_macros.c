//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !IS_WHITESPACE(input[i]) && j < MAX_WORD_LEN - 1) {
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
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        if (word_len >= MAX_WORD_LEN) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        char processed[MAX_WORD_LEN];
        for (j = 0; j < word_len; j++) {
            if (j == 0) {
                processed[j] = TO_UPPER(words[i][j]);
            } else {
                processed[j] = TO_LOWER(words[i][j]);
            }
        }
        processed[word_len] = '\0';
        
        int is_palindrome = 1;
        for (j = 0, k = word_len - 1; j < k; j++, k--) {
            if (TO_LOWER(processed[j]) != TO_LOWER(processed[k])) {
                is_palindrome = 0;
                break;
            }
        }
        
        printf("Word %d: \"%s\" -> \"%s\" %s\n", 
               i + 1, words[i], processed, 
               is_palindrome ? "[PALINDROME]" : "");
    }
    
    printf("\nWord frequency analysis:\n");
    for (i = 0; i < word_count; i++) {
        int count = 1;
        if (words[i][0] == '\0') {
            continue;
        }
        
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                words[j][0] = '\0';
            }
        }
        
        if (count > 0) {
            printf("'%s': %d occurrence(s)\n", words[i], count);
        }
    }
    
    return 0;
}