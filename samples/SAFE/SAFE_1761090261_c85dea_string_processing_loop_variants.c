//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
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
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    for (k = 0; k < word_count; k++) {
        printf("%d: \"%s\"\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        char processed[MAX_WORD_LEN + 1];
        
        for (i = 0; i < word_len && i < MAX_WORD_LEN; i++) {
            processed[i] = toupper((unsigned char)words[k][i]);
        }
        processed[i] = '\0';
        
        printf("Original: %s -> Uppercase: %s\n", words[k], processed);
    }
    
    printf("\nWord lengths:\n");
    k = 0;
    do {
        if (k < word_count) {
            printf("Word %d: %zu characters\n", k + 1, strlen(words[k]));
        }
        k++;
    } while (k < word_count);
    
    printf("\nReversed words:\n");
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        char reversed[MAX_WORD_LEN + 1];
        
        for (i = 0, j = word_len - 1; i < word_len; i++, j--) {
            if (i < MAX_WORD_LEN) {
                reversed[i] = words[k][j];
            }
        }
        reversed[word_len] = '\0';
        
        printf("%s -> %s\n", words[k], reversed);
    }
    
    return 0;
}