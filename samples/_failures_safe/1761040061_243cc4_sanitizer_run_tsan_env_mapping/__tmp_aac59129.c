//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
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
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && j < MAX_WORD_LEN - 1 && !isspace((unsigned char)input[i])) {
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
    
    printf("\nProcessing words:\n");
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        int has_letter = 0;
        
        for (j = 0; j < word_len; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                has_letter = 1;
                break;
            }
        }
        
        if (has_letter) {
            for (j = 0; j < word_len; j++) {
                if (isupper((unsigned char)words[i][j])) {
                    words[i][j] = tolower((unsigned char)words[i][j]);
                }
            }
            
            for (j = 0, k = word_len - 1; j < k; j++, k--) {
                char temp = words[i][j];
                words[i][j] = words[i][k];
                words[i][k] = temp;
            }
            
            printf("Processed: %s\n", words[i]);
        } else {
            printf("Skipped (no letters): %s\n", words[i]);
        }
    }
    
    printf("\nWord count: %d\n", word_count);
    
    return 0;
}