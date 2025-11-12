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
    printf("Word length analysis:\n");
    
    int length_counts[MAX_WORD_LEN + 1] = {0};
    int total_chars = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        if (word_len <= MAX_WORD_LEN) {
            length_counts[word_len]++;
            total_chars += (int)word_len;
        }
        
        printf("Word %d: %s (length: %zu)\n", i + 1, words[i], word_len);
    }
    
    printf("\nLength distribution:\n");
    for (int i = 1; i <= MAX_WORD_LEN; i++) {
        if (length_counts[i] > 0) {
            printf("Length %d: %d words\n", i, length_counts[i]);
        }
    }
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("\nAverage word length: %.2f\n", avg_length);
    }
    
    printf("\nWords in reverse order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}