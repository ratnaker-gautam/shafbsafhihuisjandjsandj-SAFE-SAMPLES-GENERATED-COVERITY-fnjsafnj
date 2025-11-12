//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
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
        fprintf(stderr, "Empty input\n");
        return 1;
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
        
        int word_len = j - i;
        if (word_len >= 64) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        for (k = 0; k < word_len; k++) {
            words[word_count][k] = tolower((unsigned char)input[i + k]);
        }
        words[word_count][word_len] = '\0';
        word_count++;
        
        i = j;
    }
    
    if (word_count == 0) {
        fprintf(stderr, "No words found\n");
        return 1;
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[64];
                if (strlen(words[j]) < 64 && strlen(words[j + 1]) < 64) {
                    strcpy(temp, words[j]);
                    strcpy(words[j], words[j + 1]);
                    strcpy(words[j + 1], temp);
                }
            }
        }
    }
    
    int unique_count = 0;
    for (i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            unique_count++;
        }
    }
    
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n", unique_count);
    printf("Sorted unique words:\n");
    
    for (i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            printf("%s\n", words[i]);
        }
    }
    
    return 0;
}