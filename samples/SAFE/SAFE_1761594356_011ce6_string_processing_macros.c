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
    printf("1. Original words\n");
    printf("2. Uppercase words\n");
    printf("3. Lowercase words\n");
    printf("4. Capitalized words\n");
    printf("5. Reversed words\n\n");
    
    for (i = 0; i < word_count; i++) {
        printf("%-20s", words[i]);
        
        char temp[MAX_WORD_LEN];
        size_t len = strlen(words[i]);
        
        if (len >= sizeof(temp)) {
            fprintf(stderr, "Word too long for processing\n");
            return 1;
        }
        
        for (j = 0; j < len; j++) {
            temp[j] = TO_UPPER(words[i][j]);
        }
        temp[j] = '\0';
        printf("%-20s", temp);
        
        for (j = 0; j < len; j++) {
            temp[j] = TO_LOWER(words[i][j]);
        }
        temp[j] = '\0';
        printf("%-20s", temp);
        
        strncpy(temp, words[i], sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0';
        if (len > 0) {
            temp[0] = TO_UPPER(temp[0]);
            for (j = 1; j < len; j++) {
                temp[j] = TO_LOWER(temp[j]);
            }
        }
        printf("%-20s", temp);
        
        for (j = 0, k = len - 1; j < len; j++, k--) {
            temp[j] = words[i][k];
        }
        temp[j] = '\0';
        printf("%-20s\n", temp);
    }
    
    return 0;
}