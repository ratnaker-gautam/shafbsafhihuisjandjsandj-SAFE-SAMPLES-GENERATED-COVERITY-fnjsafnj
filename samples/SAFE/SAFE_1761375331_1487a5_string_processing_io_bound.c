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
        if (token_len > 0 && token_len <= MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        char processed[MAX_WORD_LEN + 1];
        size_t len = strlen(words[i]);
        size_t processed_idx = 0;
        
        for (size_t j = 0; j < len && processed_idx < MAX_WORD_LEN; j++) {
            char c = words[i][j];
            if (isalnum((unsigned char)c)) {
                processed[processed_idx++] = tolower((unsigned char)c);
            }
        }
        processed[processed_idx] = '\0';
        
        if (processed_idx > 0) {
            printf("Original: %-15s Processed: %s\n", words[i], processed);
        } else {
            printf("Original: %-15s Processed: [empty]\n", words[i]);
        }
    }
    
    printf("\nWord frequency analysis:\n");
    int processed_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (strlen(words[i]) == 0) continue;
        
        char temp[MAX_WORD_LEN + 1];
        size_t len = strlen(words[i]);
        size_t temp_idx = 0;
        
        for (size_t j = 0; j < len && temp_idx < MAX_WORD_LEN; j++) {
            char c = words[i][j];
            if (isalnum((unsigned char)c)) {
                temp[temp_idx++] = tolower((unsigned char)c);
            }
        }
        temp[temp_idx] = '\0';
        
        if (temp_idx == 0) continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            char temp2[MAX_WORD_LEN + 1];
            size_t len2 = strlen(words[j]);
            size_t temp2_idx = 0;
            
            for (size_t k = 0; k < len2 && temp2_idx < MAX_WORD_LEN; k++) {
                char c = words[j][k];
                if (isalnum((unsigned char)c)) {
                    temp2[temp2_idx++] = tolower((unsigned char)c);
                }
            }
            temp2[temp2_idx] = '\0';
            
            if (temp2_idx > 0 && strcmp(temp, temp2) == 0) {
                count++;
                words[j][0] = '\0';
            }
        }
        
        if (count > 0) {
            printf("'%s': %d occurrence(s)\n", temp, count);
            processed_count++;
        }
    }
    
    if (processed_count == 0) {
        printf("No processable words found\n");
    }
    
    return 0;
}