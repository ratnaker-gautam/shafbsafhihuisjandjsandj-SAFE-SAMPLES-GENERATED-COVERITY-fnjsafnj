//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input string\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
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
        
        j = 0;
        while (i < input_len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
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
    k = 0;
    do {
        if (k >= word_count) {
            break;
        }
        
        size_t len = strlen(words[k]);
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        for (j = 0; j < len; j++) {
            unsigned char c = words[k][j];
            if (isupper(c)) has_upper = 1;
            if (islower(c)) has_lower = 1;
            if (isdigit(c)) has_digit = 1;
        }
        
        printf("Word %d: ", k + 1);
        if (has_upper && has_lower) {
            printf("Mixed case");
        } else if (has_upper) {
            printf("Uppercase");
        } else if (has_lower) {
            printf("Lowercase");
        } else {
            printf("No letters");
        }
        
        if (has_digit) {
            printf(" with digits");
        }
        printf("\n");
        
        k++;
    } while (k < word_count);
    
    printf("\nReversed words:\n");
    for (k = word_count - 1; k >= 0; k--) {
        size_t len = strlen(words[k]);
        printf("Word %d reversed: ", k + 1);
        
        for (j = len - 1; j >= 0; j--) {
            putchar(words[k][j]);
        }
        printf("\n");
    }
    
    return 0;
}