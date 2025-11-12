//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
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
        return 1;
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
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        char processed[MAX_WORD_LEN + 1];
        size_t len = strlen(words[i]);
        size_t processed_idx = 0;
        
        for (size_t j = 0; j < len && processed_idx < MAX_WORD_LEN; j++) {
            if (isalnum((unsigned char)words[i][j])) {
                processed[processed_idx++] = tolower((unsigned char)words[i][j]);
            }
        }
        processed[processed_idx] = '\0';
        
        if (processed_idx > 0) {
            printf("Word %d: Original='%s', Processed='%s', Length=%zu\n", 
                   i + 1, words[i], processed, processed_idx);
        } else {
            printf("Word %d: Original='%s' (no alphanumeric characters)\n", i + 1, words[i]);
        }
    }
    
    printf("\nWord frequency analysis:\n");
    int processed_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp[MAX_WORD_LEN + 1];
        size_t len = strlen(words[i]);
        size_t temp_idx = 0;
        
        for (size_t j = 0; j < len && temp_idx < MAX_WORD_LEN; j++) {
            if (isalnum((unsigned char)words[i][j])) {
                temp[temp_idx++] = tolower((unsigned char)words[i][j]);
            }
        }
        temp[temp_idx] = '\0';
        
        if (temp_idx == 0) continue;
        
        int is_duplicate = 0;
        for (int k = 0; k < i; k++) {
            char temp2[MAX_WORD_LEN + 1];
            size_t len2 = strlen(words[k]);
            size_t temp2_idx = 0;
            
            for (size_t l = 0; l < len2 && temp2_idx < MAX_WORD_LEN; l++) {
                if (isalnum((unsigned char)words[k][l])) {
                    temp2[temp2_idx++] = tolower((unsigned char)words[k][l]);
                }
            }
            temp2[temp2_idx] = '\0';
            
            if (temp2_idx > 0 && strcmp(temp, temp2) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            int frequency = 1;
            for (int k = i + 1; k < word_count; k++) {
                char temp2[MAX_WORD_LEN + 1];
                size_t len2 = strlen(words[k]);
                size_t temp2_idx = 0;
                
                for (size_t l = 0; l < len2 && temp2_idx < MAX_WORD_LEN; l++) {
                    if (isalnum((unsigned char)words[k][l])) {
                        temp2[temp2_idx++] = tolower((unsigned char)words[k][l]);
                    }
                }
                temp2[temp2_idx] = '\0';
                
                if (temp2_idx > 0 && strcmp(temp, temp2) == 0) {
                    frequency++;
                }
            }
            printf("'%s' appears %d time(s)\n", temp, frequency);
            processed_count++;
        }
    }
    
    printf("\nTotal unique processed words: %d\n", processed_count);
    
    return 0;
}