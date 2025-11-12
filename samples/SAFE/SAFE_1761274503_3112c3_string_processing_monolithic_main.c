//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text to process: ");
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
        
        if (word_len > 0) {
            strncpy(words[word_count], &input[i], word_len);
            words[word_count][word_len] = '\0';
            
            for (k = 0; k < word_len; k++) {
                words[word_count][k] = tolower((unsigned char)words[word_count][k]);
            }
            
            word_count++;
        }
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    
    printf("\nWords in lowercase:\n");
    for (i = 0; i < word_count; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%s", words[i]);
    }
    printf("\n");
    
    printf("\nWord lengths:\n");
    for (i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        printf("'%s': %zu characters\n", words[i], len);
    }
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += (int)strlen(words[i]);
    }
    
    printf("\nAverage word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    
    printf("\nWords sorted alphabetically:\n");
    for (i = 0; i < word_count - 1; i++) {
        for (j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) > 0) {
                char temp[64];
                strcpy(temp, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], temp);
            }
        }
    }
    
    for (i = 0; i < word_count; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%s", words[i]);
    }
    printf("\n");
    
    return 0;
}