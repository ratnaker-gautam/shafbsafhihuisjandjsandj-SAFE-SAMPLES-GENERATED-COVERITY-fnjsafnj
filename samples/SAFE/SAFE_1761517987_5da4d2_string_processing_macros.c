//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
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
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int frequencies[MAX_WORDS] = {0};
    
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
    
    size_t pos = 0;
    while (pos < input_len && word_count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) {
            break;
        }
        
        size_t word_start = pos;
        size_t word_len = 0;
        
        while (pos < input_len && !IS_WHITESPACE(input[pos]) && word_len < MAX_WORD_LEN) {
            words[word_count][word_len] = TO_LOWER(input[pos]);
            word_len++;
            pos++;
        }
        
        if (word_len > 0) {
            words[word_count][word_len] = '\0';
            
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i], words[word_count]) == 0) {
                    frequencies[i]++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                frequencies[word_count] = 1;
                word_count++;
            }
        }
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        if (frequencies[i] > 0) {
            printf("%s: %d\n", words[i], frequencies[i]);
        }
    }
    
    return 0;
}