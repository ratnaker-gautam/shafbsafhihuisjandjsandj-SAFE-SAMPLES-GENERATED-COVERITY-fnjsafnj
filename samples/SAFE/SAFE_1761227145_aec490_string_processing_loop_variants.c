//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

int main() {
    char input[MAX_INPUT + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT);
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
    
    if (len > MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
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
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        if (word_len == 0) continue;
        
        int has_digit = 0;
        int m = 0;
        do {
            if (isdigit((unsigned char)words[k][m])) {
                has_digit = 1;
                break;
            }
            m++;
        } while (m < word_len);
        
        if (!has_digit) {
            for (int n = 0; n < word_len; n++) {
                words[k][n] = toupper((unsigned char)words[k][n]);
            }
            printf("Uppercase: %s\n", words[k]);
            processed++;
        } else {
            printf("Skipped (contains digit): %s\n", words[k]);
        }
    }
    
    printf("\nTotal words processed: %d\n", processed);
    
    return 0;
}