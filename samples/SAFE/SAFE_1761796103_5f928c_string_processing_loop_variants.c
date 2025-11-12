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
        printf("Word %d: %s\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        char processed[MAX_WORD_LEN + 1];
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        for (i = 0; i < word_len; i++) {
            char c = words[k][i];
            if (isupper((unsigned char)c)) {
                has_upper = 1;
            } else if (islower((unsigned char)c)) {
                has_lower = 1;
            } else if (isdigit((unsigned char)c)) {
                has_digit = 1;
            }
        }
        
        j = 0;
        for (i = 0; i < word_len && j < MAX_WORD_LEN; i++) {
            char c = words[k][i];
            if (isalpha((unsigned char)c)) {
                if (has_upper && has_lower) {
                    processed[j] = tolower((unsigned char)c);
                } else {
                    processed[j] = c;
                }
                j++;
            } else if (isdigit((unsigned char)c)) {
                processed[j] = c;
                j++;
            }
        }
        processed[j] = '\0';
        
        printf("Original: %-15s Processed: %s\n", words[k], processed);
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int total_alpha = 0;
    int total_digits = 0;
    
    k = 0;
    while (k < word_count) {
        size_t word_len = strlen(words[k]);
        total_chars += word_len;
        
        for (i = 0; i < word_len; i++) {
            char c = words[k][i];
            if (isalpha((unsigned char)c)) {
                total_alpha++;
            } else if (isdigit((unsigned char)c)) {
                total_digits++;
            }
        }
        k++;
    }
    
    printf("Total characters in words: %d\n", total_chars);
    printf("Total alphabetic characters: %d\n", total_alpha);
    printf("Total digit characters: %d\n", total_digits);
    
    return 0;
}