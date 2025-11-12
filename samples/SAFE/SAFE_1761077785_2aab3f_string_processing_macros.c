//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    char words[MAX_WORDS][MAX_WORD_LEN + 1] = {{0}};
    int word_count = 0;
    int char_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (input[i] != '\0' && word_count < MAX_WORDS) {
        while (IS_WHITESPACE(input[i]) && input[i] != '\0') {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        int j = 0;
        while (!IS_WHITESPACE(input[i]) && input[i] != '\0' && j < MAX_WORD_LEN) {
            words[word_count][j] = TO_LOWER(input[i]);
            j++;
            i++;
        }
        
        if (j < MAX_WORD_LEN) {
            words[word_count][j] = '\0';
        } else {
            words[word_count][MAX_WORD_LEN] = '\0';
        }
        
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Processed %d words:\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        int valid = 1;
        int len = strlen(words[idx]);
        
        if (len == 0) {
            continue;
        }
        
        for (int k = 0; k < len; k++) {
            if (!isalpha((unsigned char)words[idx][k])) {
                valid = 0;
                break;
            }
        }
        
        if (valid) {
            printf("%s\n", words[idx]);
            char_count += len;
        }
    }
    
    printf("Total alphabetic characters in valid words: %d\n", char_count);
    
    return 0;
}