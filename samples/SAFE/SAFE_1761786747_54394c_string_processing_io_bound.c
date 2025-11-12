//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int i, j;
    
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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (input[i] != '\0' && word_count < MAX_WORDS) {
        while (input[i] != '\0' && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (input[i] == '\0') {
            break;
        }
        
        j = 0;
        while (input[i] != '\0' && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN - 1) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j >= MAX_WORD_LEN - 1) {
            while (input[i] != '\0' && !isspace((unsigned char)input[i])) {
                i++;
            }
        }
        
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nWord count: %d\n", word_count);
    printf("Words (reversed):\n");
    
    for (i = word_count - 1; i >= 0; i--) {
        size_t word_len = strlen(words[i]);
        if (word_len > 0) {
            printf("%s\n", words[i]);
        }
    }
    
    printf("\nWord lengths:\n");
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        printf("'%s': %zu characters\n", words[i], word_len);
    }
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += (int)strlen(words[i]);
    }
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("\nAverage word length: %.2f\n", avg_length);
    }
    
    return 0;
}