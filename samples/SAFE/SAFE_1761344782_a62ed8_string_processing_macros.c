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
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

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
        while (IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        j = 0;
        while (input[i] != '\0' && !IS_WHITESPACE(input[i]) && j < MAX_WORD_LEN - 1) {
            if (IS_ALPHA(input[i])) {
                words[word_count][j] = TO_LOWER(input[i]);
                j++;
            }
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("Processed words (%d found):\n", word_count);
    for (i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord frequency analysis:\n");
    int processed[MAX_WORDS] = {0};
    
    for (i = 0; i < word_count; i++) {
        if (processed[i]) {
            continue;
        }
        
        int count = 1;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s': %d occurrence(s)\n", words[i], count);
        processed[i] = 1;
    }
    
    printf("\nSorted by frequency (descending):\n");
    char sorted_words[MAX_WORDS][MAX_WORD_LEN];
    int frequencies[MAX_WORDS];
    int sorted_count = 0;
    
    for (i = 0; i < word_count; i++) {
        if (i > 0) {
            for (j = 0; j < sorted_count; j++) {
                if (strcmp(words[i], sorted_words[j]) == 0) {
                    break;
                }
            }
            if (j < sorted_count) {
                continue;
            }
        }
        
        int count = 1;
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
            }
        }
        
        strcpy(sorted_words[sorted_count], words[i]);
        frequencies[sorted_count] = count;
        sorted_count++;
    }
    
    for (i = 0; i < sorted_count - 1; i++) {
        for (j = 0; j < sorted_count - i - 1; j++) {
            if (frequencies[j] < frequencies[j + 1]) {
                int temp_freq = frequencies[j];
                frequencies[j] = frequencies[j + 1];
                frequencies[j + 1] = temp_freq;
                
                char temp_word[MAX_WORD_LEN];
                strcpy(temp_word, sorted_words[j]);
                strcpy(sorted_words[j], sorted_words[j + 1]);
                strcpy(sorted_words[j + 1], temp_word);
            }
        }
    }
    
    for (i = 0; i < sorted_count; i++) {
        printf("'%s': %d\n", sorted_words[i], frequencies[i]);
    }
    
    return 0;
}