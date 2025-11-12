//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_INPUT_LEN];
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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = i;
        while (j < input_len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        size_t word_len = j - i;
        if (word_len >= MAX_INPUT_LEN) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        for (k = 0; k < word_len; k++) {
            words[word_count][k] = input[i + k];
        }
        words[word_count][word_len] = '\0';
        word_count++;
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_INPUT_LEN];
                strncpy(temp, words[j], sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
                strncpy(words[j], words[j + 1], sizeof(words[j]) - 1);
                words[j][sizeof(words[j]) - 1] = '\0';
                strncpy(words[j + 1], temp, sizeof(words[j + 1]) - 1);
                words[j + 1][sizeof(words[j + 1]) - 1] = '\0';
            }
        }
    }
    
    printf("Sorted words:\n");
    for (i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    int unique_count = 0;
    for (i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            unique_count++;
        }
    }
    
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n", unique_count);
    
    return 0;
}