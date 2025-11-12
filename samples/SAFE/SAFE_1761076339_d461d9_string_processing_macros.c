//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a sentence: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (input[i] != '\0' && word_count < MAX_WORDS) {
        while (IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        j = 0;
        while (input[i] != '\0' && !IS_WHITESPACE(input[i]) && j < MAX_WORD_LEN - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    if (word_count == 0) {
        fprintf(stderr, "No words found\n");
        return 1;
    }
    
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > 0) {
            words[i][0] = TO_UPPER(words[i][0]);
            for (j = 1; j < len; j++) {
                words[i][j] = TO_LOWER(words[i][j]);
            }
            
            if (IS_TERMINATOR(words[i][len - 1])) {
                for (k = 0; k < word_count; k++) {
                    if (k == i) continue;
                    size_t other_len = strlen(words[k]);
                    if (other_len > 0) {
                        words[k][0] = TO_LOWER(words[k][0]);
                    }
                }
            }
        }
    }
    
    printf("Processed text: ");
    for (i = 0; i < word_count; i++) {
        printf("%s", words[i]);
        if (i < word_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    return 0;
}