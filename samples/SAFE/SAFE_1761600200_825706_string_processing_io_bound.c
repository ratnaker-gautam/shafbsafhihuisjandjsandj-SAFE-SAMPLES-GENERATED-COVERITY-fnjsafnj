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
        size_t k = 0;
        
        while (words[i][j] != '\0' && k < MAX_WORD_LEN) {
            if (isalpha((unsigned char)words[i][j])) {
                processed[k] = tolower((unsigned char)words[i][j]);
                k++;
            }
            j++;
        }
        processed[k] = '\0';
        
        if (k > 0) {
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
        size_t k = 0;
        
        while (words[i][j] != '\0' && k < MAX_WORD_LEN) {
            if (isalpha((unsigned char)words[i][j])) {
                temp[k] = tolower((unsigned char)words[i][j]);
                k++;
            }
            j++;
        }
        temp[k] = '\0';
        
        if (k == 0) continue;
        
        int counted = 0;
        for (int m = 0; m < i; m++) {
            char temp2[MAX_WORD_LEN + 1];
            size_t n = 0;
            size_t p = 0;
            
            while (words[m][n] != '\0' && p < MAX_WORD_LEN) {
                if (isalpha((unsigned char)words[m][n])) {
                    temp2[p] = tolower((unsigned char)words[m][n]);
                    p++;
                }
                n++;
            }
            temp2[p] = '\0';
            
            if (p > 0 && strcmp(temp, temp2) == 0) {
                counted = 1;
                break;
            }
        }
        
        if (!counted) {
            int freq = 1;
            for (int m = i + 1; m < word_count; m++) {
                char temp2[MAX_WORD_LEN + 1];
                size_t n = 0;
                size_t p = 0;
                
                while (words[m][n] != '\0' && p < MAX_WORD_LEN) {
                    if (isalpha((unsigned char)words[m][n])) {
                        temp2[p] = tolower((unsigned char)words[m][n]);
                        p++;
                    }
                    n++;
                }
                temp2[p] = '\0';
                
                if (p > 0 && strcmp(temp, temp2) == 0) {
                    freq++;
                }
            }
            printf("'%s': %d occurrence(s)\n", temp, freq);
            processed_count++;
        }
    }
    
    if (processed_count == 0) {
        printf("No valid words to analyze\n");
    }
    
    return 0;
}