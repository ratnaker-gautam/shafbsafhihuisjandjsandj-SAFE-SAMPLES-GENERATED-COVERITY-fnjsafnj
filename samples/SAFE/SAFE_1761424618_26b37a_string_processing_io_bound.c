//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

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
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    printf("Word length analysis:\n");
    
    int length_counts[20] = {0};
    int total_chars = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        if (word_len > 0) {
            total_chars += (int)word_len;
            if (word_len < 20) {
                length_counts[word_len]++;
            } else {
                length_counts[19]++;
            }
            
            for (size_t j = 0; j < word_len; j++) {
                words[i][j] = (char)toupper((unsigned char)words[i][j]);
            }
        }
    }
    
    for (int i = 1; i < 20; i++) {
        if (length_counts[i] > 0) {
            printf("Length %d: %d words\n", i, length_counts[i]);
        }
    }
    if (length_counts[19] > 0) {
        printf("Length 19+: %d words\n", length_counts[19]);
    }
    
    printf("\nAverage word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    
    printf("\nWords in uppercase:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }
    if (word_count % 5 != 0) {
        printf("\n");
    }
    
    printf("\nWord frequency analysis:\n");
    int processed[MAX_WORDS] = {0};
    
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s': %d occurrence(s)\n", words[i], count);
        processed[i] = 1;
    }
    
    return 0;
}