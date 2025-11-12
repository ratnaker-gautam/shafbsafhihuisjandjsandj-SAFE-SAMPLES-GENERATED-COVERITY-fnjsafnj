//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
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
        return 0;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len >= MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            token = strtok(NULL, " \t\n\r");
            continue;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN - 1);
        words[word_count][MAX_WORD_LEN - 1] = '\0';
        word_count++;
        
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        char processed[MAX_WORD_LEN];
        size_t len = strlen(words[i]);
        size_t out_idx = 0;
        
        for (size_t j = 0; j < len && out_idx < MAX_WORD_LEN - 1; j++) {
            if (isalnum((unsigned char)words[i][j])) {
                processed[out_idx++] = tolower((unsigned char)words[i][j]);
            }
        }
        processed[out_idx] = '\0';
        
        if (out_idx > 0) {
            printf("Word %d: '%s' -> '%s' (length: %zu)\n", 
                   i + 1, words[i], processed, out_idx);
        } else {
            printf("Word %d: '%s' -> [empty]\n", i + 1, words[i]);
        }
    }
    
    printf("\nWord frequency analysis:\n");
    int processed_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (strlen(words[i]) == 0) continue;
        
        char temp[MAX_WORD_LEN];
        size_t len = strlen(words[i]);
        size_t out_idx = 0;
        
        for (size_t j = 0; j < len && out_idx < MAX_WORD_LEN - 1; j++) {
            if (isalnum((unsigned char)words[i][j])) {
                temp[out_idx++] = tolower((unsigned char)words[i][j]);
            }
        }
        temp[out_idx] = '\0';
        
        if (out_idx == 0) continue;
        
        int count = 1;
        int counted = 0;
        
        for (int k = 0; k < i; k++) {
            char temp2[MAX_WORD_LEN];
            size_t len2 = strlen(words[k]);
            size_t out_idx2 = 0;
            
            for (size_t l = 0; l < len2 && out_idx2 < MAX_WORD_LEN - 1; l++) {
                if (isalnum((unsigned char)words[k][l])) {
                    temp2[out_idx2++] = tolower((unsigned char)words[k][l]);
                }
            }
            temp2[out_idx2] = '\0';
            
            if (out_idx2 > 0 && strcmp(temp, temp2) == 0) {
                counted = 1;
                break;
            }
        }
        
        if (!counted) {
            for (int k = i + 1; k < word_count; k++) {
                char temp2[MAX_WORD_LEN];
                size_t len2 = strlen(words[k]);
                size_t out_idx2 = 0;
                
                for (size_t l = 0; l < len2 && out_idx2 < MAX_WORD_LEN - 1; l++) {
                    if (isalnum((unsigned char)words[k][l])) {
                        temp2[out_idx2++] = tolower((unsigned char)words[k][l]);
                    }
                }
                temp2[out_idx2] = '\0';
                
                if (out_idx2 > 0 && strcmp(temp, temp2) == 0) {
                    count++;
                }
            }
            printf("'%s': %d occurrence(s)\n", temp, count);
            processed_count++;
        }
    }
    
    if (processed_count ==