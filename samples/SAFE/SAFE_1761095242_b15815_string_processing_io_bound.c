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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
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
        size_t j = 0;
        
        for (size_t k = 0; words[i][k] != '\0' && j < MAX_WORD_LEN; k++) {
            if (isalnum((unsigned char)words[i][k])) {
                processed[j++] = tolower((unsigned char)words[i][k]);
            }
        }
        processed[j] = '\0';
        
        if (j > 0) {
            printf("Original: %-15s Processed: %s\n", words[i], processed);
        } else {
            printf("Original: %-15s Processed: [empty]\n", words[i]);
        }
    }
    
    printf("\nWord frequency analysis:\n");
    int processed_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp[MAX_WORD_LEN + 1];
        size_t j = 0;
        
        for (size_t k = 0; words[i][k] != '\0' && j < MAX_WORD_LEN; k++) {
            if (isalnum((unsigned char)words[i][k])) {
                temp[j++] = tolower((unsigned char)words[i][k]);
            }
        }
        temp[j] = '\0';
        
        if (j == 0) continue;
        
        int duplicate = 0;
        for (int k = 0; k < i; k++) {
            char temp2[MAX_WORD_LEN + 1];
            size_t m = 0;
            
            for (size_t n = 0; words[k][n] != '\0' && m < MAX_WORD_LEN; n++) {
                if (isalnum((unsigned char)words[k][n])) {
                    temp2[m++] = tolower((unsigned char)words[k][n]);
                }
            }
            temp2[m] = '\0';
            
            if (m > 0 && strcmp(temp, temp2) == 0) {
                duplicate = 1;
                break;
            }
        }
        
        if (!duplicate) {
            int freq = 1;
            for (int k = i + 1; k < word_count; k++) {
                char temp2[MAX_WORD_LEN + 1];
                size_t m = 0;
                
                for (size_t n = 0; words[k][n] != '\0' && m < MAX_WORD_LEN; n++) {
                    if (isalnum((unsigned char)words[k][n])) {
                        temp2[m++] = tolower((unsigned char)words[k][n]);
                    }
                }
                temp2[m] = '\0';
                
                if (m > 0 && strcmp(temp, temp2) == 0) {
                    freq++;
                }
            }
            
            printf("'%s': %d occurrence(s)\n", temp, freq);
            processed_count++;
        }
    }
    
    printf("\nTotal unique processed words: %d\n", processed_count);
    
    return 0;
}