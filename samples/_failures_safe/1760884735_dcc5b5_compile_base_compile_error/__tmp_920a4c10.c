//DeepSeek-V3 SAFE v1.3 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_INPUT_LEN / 2][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
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
    
    i = 0;
    while (i < input_len && word_count < (MAX_INPUT_LEN / 2)) {
        while (i < input_len && isspace(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace(input[i]) && j < MAX_WORD_LEN) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("\nWords found: %d\n", word_count);
    printf("Words in reverse order:\n");
    
    for (k = word_count - 1; k >= 0; k--) {
        if (printf("%s\n", words[k]) < 0) {
            fprintf(stderr, "Error writing output\n");
            return 1;
        }
    }
    
    printf("\nWord statistics:\n");
    printf("Total characters in words: ");
    
    int total_chars = 0;
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        if (total_chars > INT_MAX - word_len) {
            fprintf(stderr, "Integer overflow detected\n");
            return 1;
        }
        total_chars += word_len;
    }
    
    printf("%d\n", total_chars);
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("Average word length: %.2f\n", avg_length);
    }
    
    return 0;
}