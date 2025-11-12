//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int total_chars = 0;
    int total_words = 0;
    int longest_word_len = 0;
    
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
        
        if (token_len > 63) {
            fprintf(stderr, "Word too long: %s\n", token);
            return 1;
        }
        
        strncpy(words[word_count], token, 63);
        words[word_count][63] = '\0';
        
        total_chars += (int)token_len;
        total_words++;
        
        if ((int)token_len > longest_word_len) {
            longest_word_len = (int)token_len;
        }
        
        word_count++;
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nText analysis results:\n");
    printf("Total characters (words only): %d\n", total_chars);
    printf("Total words: %d\n", total_words);
    printf("Average word length: %.2f\n", total_words > 0 ? (double)total_chars / total_words : 0.0);
    printf("Longest word length: %d\n", longest_word_len);
    
    printf("\nWords in reverse order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("\nWords sorted by length:\n");
    int sorted_indices[MAX_WORDS];
    for (int i = 0; i < word_count; i++) {
        sorted_indices[i] = i;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strlen(words[sorted_indices[i]]) > strlen(words[sorted_indices[j]])) {
                int temp = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[sorted_indices[i]]);
    }
    printf("\n");
    
    return 0;
}