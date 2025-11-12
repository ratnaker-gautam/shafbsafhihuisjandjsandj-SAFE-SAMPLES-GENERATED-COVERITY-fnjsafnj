//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
    while (i < len) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
            if (word_count >= MAX_WORDS) {
                fprintf(stderr, "Too many words\n");
                return 1;
            }
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
        printf("%d: '%s'\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        if (word_len == 0) {
            continue;
        }
        
        int has_digit = 0;
        int m = 0;
        do {
            if (isdigit((unsigned char)words[k][m])) {
                has_digit = 1;
                break;
            }
            m++;
        } while (m < word_len);
        
        if (has_digit) {
            int n = 0;
            while (n < word_len) {
                if (isalpha((unsigned char)words[k][n])) {
                    words[k][n] = toupper((unsigned char)words[k][n]);
                }
                n++;
            }
            printf("Modified '%s' (contains digits)\n", words[k]);
        } else {
            printf("Kept '%s' unchanged\n", words[k]);
        }
        processed++;
    }
    
    printf("\nFinal word list:\n");
    int idx = 0;
    while (idx < word_count) {
        printf("%s", words[idx]);
        if (idx < word_count - 1) {
            printf(" ");
        }
        idx++;
    }
    printf("\n");
    
    return 0;
}