//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
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
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
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
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nWord frequency analysis:\n");
    printf("=======================\n");
    
    for (i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') {
            continue;
        }
        
        int count = 1;
        for (j = i + 1; j < word_count; j++) {
            if (words[j][0] != '\0' && strcmp(words[i], words[j]) == 0) {
                count++;
                words[j][0] = '\0';
            }
        }
        
        printf("'%s': %d occurrence(s)\n", words[i], count);
    }
    
    printf("\nSorted by frequency:\n");
    printf("===================\n");
    
    char temp_word[MAX_WORD_LEN];
    int temp_count;
    int counts[MAX_WORDS];
    
    for (i = 0; i < word_count; i++) {
        counts[i] = 1;
        if (words[i][0] == '\0') {
            continue;
        }
        for (j = i + 1; j < word_count; j++) {
            if (words[j][0] != '\0' && strcmp(words[i], words[j]) == 0) {
                counts[i]++;
                words[j][0] = '\0';
            }
        }
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (words[i][0] != '\0' && words[j][0] != '\0' && counts[i] < counts[j]) {
                temp_count = counts[i];
                counts[i] = counts[j];
                counts[j] = temp_count;
                
                strncpy(temp_word, words[i], MAX_WORD_LEN - 1);
                temp_word[MAX_WORD_LEN - 1] = '\0';
                strncpy(words[i], words[j], MAX_WORD_LEN - 1);
                words[i][MAX_WORD_LEN - 1] = '\0';
                strncpy(words[j], temp_word, MAX_WORD_LEN - 1);
                words[j][MAX_WORD_LEN - 1] = '\0';
            }
        }
    }
    
    for (i = 0; i < word_count; i++) {
        if (words[i][0] != '\0') {
            printf("'%s': %d occurrence(s)\n", words[i], counts[i]);
        }
    }
    
    return 0;
}