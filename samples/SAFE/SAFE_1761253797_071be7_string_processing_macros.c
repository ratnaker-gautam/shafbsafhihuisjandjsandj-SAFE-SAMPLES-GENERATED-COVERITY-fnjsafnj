//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))

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
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (input[i] != '\0' && word_count < MAX_WORDS) {
        while (IS_WHITESPACE(input[i]) && input[i] != '\0') {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        j = 0;
        while (!IS_WHITESPACE(input[i]) && input[i] != '\0' && j < MAX_WORD_LEN - 1) {
            words[word_count][j] = TO_LOWER(input[i]);
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
    
    printf("Processed words (%d found):\n", word_count);
    for (i = 0; i < word_count; i++) {
        int is_duplicate = 0;
        for (j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            int count = 1;
            for (k = i + 1; k < word_count; k++) {
                if (strcmp(words[i], words[k]) == 0) {
                    count++;
                }
            }
            printf("'%s': %d occurrence(s)\n", words[i], count);
        }
    }
    
    return 0;
}