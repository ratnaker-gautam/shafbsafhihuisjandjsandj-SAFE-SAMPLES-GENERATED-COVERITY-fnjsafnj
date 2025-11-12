//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

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
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace(input[i]) && j < MAX_WORD_LEN) {
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
        printf("Word %d: %s\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    int processed = 0;
    do {
        if (processed >= word_count) {
            break;
        }
        
        char current[MAX_WORD_LEN + 1];
        strncpy(current, words[processed], sizeof(current) - 1);
        current[sizeof(current) - 1] = '\0';
        
        int has_digit = 0;
        for (int m = 0; current[m] != '\0'; m++) {
            if (isdigit(current[m])) {
                has_digit = 1;
                break;
            }
        }
        
        if (has_digit) {
            printf("Word '%s' contains digits - converting to uppercase\n", current);
            for (int n = 0; current[n] != '\0'; n++) {
                current[n] = toupper(current[n]);
            }
            printf("Result: %s\n", current);
        } else {
            printf("Word '%s' contains no digits\n", current);
        }
        
        processed++;
    } while (processed < word_count);
    
    printf("\nWord lengths:\n");
    int idx = 0;
    for (;;) {
        if (idx >= word_count) {
            break;
        }
        printf("'%s' has %zu characters\n", words[idx], strlen(words[idx]));
        idx++;
    }
    
    return 0;
}