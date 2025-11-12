//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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
        printf("No input provided\n");
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
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        char processed[MAX_WORD_LEN + 1];
        size_t len = strlen(words[i]);
        size_t out_idx = 0;
        
        for (size_t j = 0; j < len && out_idx < MAX_WORD_LEN; j++) {
            char c = words[i][j];
            if (isalpha((unsigned char)c)) {
                processed[out_idx++] = tolower((unsigned char)c);
            } else if (isdigit((unsigned char)c)) {
                processed[out_idx++] = c;
            }
        }
        processed[out_idx] = '\0';
        
        if (out_idx > 0) {
            printf("Original: %-20s Processed: %s\n", words[i], processed);
        } else {
            printf("Original: %-20s Processed: [empty]\n", words[i]);
        }
    }
    
    printf("\nWord frequency analysis:\n");
    int processed_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp[MAX_WORD_LEN + 1];
        size_t len = strlen(words[i]);
        size_t out_idx = 0;
        
        for (size_t j = 0; j < len && out_idx < MAX_WORD_LEN; j++) {
            char c = words[i][j];
            if (isalpha((unsigned char)c)) {
                temp[out_idx++] = tolower((unsigned char)c);
            } else if (isdigit((unsigned char)c)) {
                temp[out_idx++] = c;
            }
        }
        temp[out_idx] = '\0';
        
        if (out_idx == 0) {
            continue;
        }
        
        int count = 1;
        int counted = 0;
        
        for (int k = 0; k < i; k++) {
            char temp2[MAX_WORD_LEN + 1];
            size_t len2 = strlen(words[k]);
            size_t out_idx2 = 0;
            
            for (size_t m = 0; m < len2 && out_idx2 < MAX_WORD_LEN; m++) {
                char c = words[k][m];
                if (isalpha((unsigned char)c)) {
                    temp2[out_idx2++] = tolower((unsigned char)c);
                } else if (isdigit((unsigned char)c)) {
                    temp2[out_idx2++] = c;
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
                char temp2[MAX_WORD_LEN + 1];
                size_t len2 = strlen(words[k]);
                size_t out_idx2 = 0;
                
                for (size_t m = 0; m < len2 && out_idx2 < MAX_WORD_LEN; m++) {
                    char c = words[k][m];
                    if (isalpha((unsigned char)c)) {
                        temp2[out_idx2++] = tolower((unsigned char)c);
                    } else if (isdigit((unsigned char)c)) {
                        temp2[out_idx2++] = c;
                    }
                }
                temp2[out_idx2] = '\0';
                
                if (out_idx2 > 0 && strcmp(temp, temp2) ==