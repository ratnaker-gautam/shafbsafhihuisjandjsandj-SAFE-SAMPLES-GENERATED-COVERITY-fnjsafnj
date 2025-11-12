//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    
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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            return 1;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN);
        words[word_count][MAX_WORD_LEN] = '\0';
        word_count++;
        
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        char processed[MAX_WORD_LEN + 1];
        size_t len = strlen(words[i]);
        size_t j = 0;
        
        for (size_t k = 0; k < len && j < MAX_WORD_LEN; k++) {
            if (isalnum((unsigned char)words[i][k])) {
                processed[j++] = tolower((unsigned char)words[i][k]);
            }
        }
        processed[j] = '\0';
        
        if (j > 0) {
            printf("Word %d: '%s' -> '%s' (%zu chars)\n", 
                   i + 1, words[i], processed, j);
        } else {
            printf("Word %d: '%s' -> [empty]\n", i + 1, words[i]);
        }
    }
    
    printf("\nWord frequency analysis:\n");
    int freq[MAX_WORDS] = {0};
    int processed[MAX_WORDS] = {0};
    
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        char temp1[MAX_WORD_LEN + 1];
        size_t len1 = strlen(words[i]);
        size_t idx1 = 0;
        for (size_t k = 0; k < len1; k++) {
            if (isalnum((unsigned char)words[i][k])) {
                temp1[idx1++] = tolower((unsigned char)words[i][k]);
            }
        }
        temp1[idx1] = '\0';
        
        if (idx1 == 0) continue;
        
        freq[i] = 1;
        processed[i] = 1;
        
        for (int j = i + 1; j < word_count; j++) {
            if (processed[j]) continue;
            
            char temp2[MAX_WORD_LEN + 1];
            size_t len2 = strlen(words[j]);
            size_t idx2 = 0;
            for (size_t k = 0; k < len2; k++) {
                if (isalnum((unsigned char)words[j][k])) {
                    temp2[idx2++] = tolower((unsigned char)words[j][k]);
                }
            }
            temp2[idx2] = '\0';
            
            if (idx2 == 0) continue;
            
            if (strcmp(temp1, temp2) == 0) {
                freq[i]++;
                processed[j] = 1;
            }
        }
        
        if (freq[i] > 0) {
            printf("'%s': %d occurrences\n", temp1, freq[i]);
        }
    }
    
    return 0;
}