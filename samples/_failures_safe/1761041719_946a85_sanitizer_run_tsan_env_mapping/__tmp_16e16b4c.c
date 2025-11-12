//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    if (len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < 63) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nWord count: %d\n", word_count);
    printf("Words (reversed):\n");
    
    for (i = word_count - 1; i >= 0; i--) {
        size_t word_len = strlen(words[i]);
        for (j = word_len - 1; j >= 0; j--) {
            putchar(words[i][j]);
        }
        putchar('\n');
    }
    
    printf("\nWord lengths:\n");
    for (i = 0; i < word_count; i++) {
        printf("'%s': %zu characters\n", words[i], strlen(words[i]));
    }
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += (int)strlen(words[i]);
    }
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (double)total_chars / word_count);
    }
    
    return 0;
}