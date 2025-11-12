//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
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
        while (i < input_len && j < MAX_WORD_LEN - 1 && !IS_WHITESPACE(input[i])) {
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
    
    printf("\nProcessing results:\n");
    printf("1. Word count: %d\n", word_count);
    
    printf("2. Words in reverse order:\n");
    for (i = word_count - 1; i >= 0; i--) {
        printf("   %s\n", words[i]);
    }
    
    printf("3. Capitalized words:\n");
    for (i = 0; i < word_count; i++) {
        char temp[MAX_WORD_LEN];
        size_t len = strlen(words[i]);
        
        if (len >= sizeof(temp)) {
            fprintf(stderr, "Word too long for processing\n");
            return 1;
        }
        
        strncpy(temp, words[i], sizeof(temp) - 1);
        temp[sizeof(temp) - 1] = '\0';
        
        if (len > 0) {
            temp[0] = TO_UPPER(temp[0]);
            for (j = 1; j < len; j++) {
                temp[j] = TO_LOWER(temp[j]);
            }
        }
        printf("   %s\n", temp);
    }
    
    printf("4. Words ending with punctuation:\n");
    int found_punct = 0;
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > 0 && IS_TERMINATOR(words[i][len - 1])) {
            printf("   %s\n", words[i]);
            found_punct = 1;
        }
    }
    if (!found_punct) {
        printf("   (none)\n");
    }
    
    printf("5. Longest word(s):\n");
    size_t max_len = 0;
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
        }
    }
    
    if (max_len > 0) {
        for (i = 0; i < word_count; i++) {
            if (strlen(words[i]) == max_len) {
                printf("   %s\n", words[i]);
            }
        }
    }
    
    return 0;
}