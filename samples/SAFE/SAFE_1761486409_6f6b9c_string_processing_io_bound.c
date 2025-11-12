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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
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
            printf("Original: %-20s Processed: %s\n", words[i], processed);
        } else {
            printf("Original: %-20s Processed: [no letters]\n", words[i]);
        }
    }
    
    printf("\nWord frequency analysis:\n");
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp1[MAX_WORD_LEN + 1];
        size_t idx1 = 0;
        for (size_t c = 0; words[i][c] != '\0' && idx1 < MAX_WORD_LEN; c++) {
            if (isalpha((unsigned char)words[i][c])) {
                temp1[idx1] = tolower((unsigned char)words[i][c]);
                idx1++;
            }
        }
        temp1[idx1] = '\0';
        
        if (idx1 == 0) continue;
        
        int counted = 0;
        for (int j = 0; j < i; j++) {
            char temp2[MAX_WORD_LEN + 1];
            size_t idx2 = 0;
            for (size_t c = 0; words[j][c] != '\0' && idx2 < MAX_WORD_LEN; c++) {
                if (isalpha((unsigned char)words[j][c])) {
                    temp2[idx2] = tolower((unsigned char)words[j][c]);
                    idx2++;
                }
            }
            temp2[idx2] = '\0';
            
            if (idx2 > 0 && strcmp(temp1, temp2) == 0) {
                counted = 1;
                break;
            }
        }
        
        if (!counted) {
            int freq = 1;
            for (int j = i + 1; j < word_count; j++) {
                char temp2[MAX_WORD_LEN + 1];
                size_t idx2 = 0;
                for (size_t c = 0; words[j][c] != '\0' && idx2 < MAX_WORD_LEN; c++) {
                    if (isalpha((unsigned char)words[j][c])) {
                        temp2[idx2] = tolower((unsigned char)words[j][c]);
                        idx2++;
                    }
                }
                temp2[idx2] = '\0';
                
                if (idx2 > 0 && strcmp(temp1, temp2) == 0) {
                    freq++;
                }
            }
            printf("'%s': %d occurrence(s)\n", temp1, freq);
            unique_count++;
        }
    }
    
    printf("Total unique processed words: %d\n", unique_count);
    
    return 0;
}