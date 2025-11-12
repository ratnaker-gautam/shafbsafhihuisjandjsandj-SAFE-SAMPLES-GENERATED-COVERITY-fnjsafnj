//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN + 1];
    char *words[MAX_WORDS];
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
        if (token_len > 0) {
            words[word_count] = malloc(token_len + 1);
            if (words[word_count] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                return 1;
            }
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    printf("Word length analysis:\n");
    
    int max_len = 0;
    int min_len = 1000;
    int total_len = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        total_len += (int)word_len;
        
        if ((int)word_len > max_len) {
            max_len = (int)word_len;
        }
        if ((int)word_len < min_len) {
            min_len = (int)word_len;
        }
        
        printf("Word %d: '%-15s' Length: %zu", i + 1, words[i], word_len);
        
        if (word_len > 10) {
            printf(" (long)");
        } else if (word_len < 3) {
            printf(" (short)");
        }
        printf("\n");
    }
    
    if (word_count > 0) {
        double avg_len = (double)total_len / word_count;
        printf("\nStatistics:\n");
        printf("Shortest word: %d characters\n", min_len);
        printf("Longest word: %d characters\n", max_len);
        printf("Average word length: %.2f characters\n", avg_len);
        printf("Total characters in words: %d\n", total_len);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    
    return 0;
}