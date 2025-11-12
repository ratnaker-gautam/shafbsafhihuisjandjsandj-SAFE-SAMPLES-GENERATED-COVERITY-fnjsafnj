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
    int processed = 0;
    do {
        if (processed >= word_count) {
            break;
        }
        
        char current[MAX_WORD_LEN + 1];
        strncpy(current, words[processed], sizeof(current) - 1);
        current[sizeof(current) - 1] = '\0';
        
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        for (int pos = 0; current[pos] != '\0'; pos++) {
            unsigned char c = current[pos];
            if (isupper(c)) {
                has_upper = 1;
            } else if (islower(c)) {
                has_lower = 1;
            } else if (isdigit(c)) {
                has_digit = 1;
            }
        }
        
        printf("Word '%s': ", current);
        if (has_upper && has_lower && has_digit) {
            printf("Mixed case with digits\n");
        } else if (has_upper && has_lower) {
            printf("Mixed case\n");
        } else if (has_upper) {
            printf("All uppercase\n");
        } else if (has_lower) {
            printf("All lowercase\n");
        } else if (has_digit) {
            printf("Digits only\n");
        } else {
            printf("Other characters\n");
        }
        
        processed++;
    } while (processed < word_count);
    
    printf("\nReversed words:\n");
    for (int idx = 0; idx < word_count; idx++) {
        char reversed[MAX_WORD_LEN + 1];
        int word_len = strlen(words[idx]);
        
        if (word_len > MAX_WORD_LEN) {
            fprintf(stderr, "Word length error\n");
            return 1;
        }
        
        int rev_idx = 0;
        for (int pos = word_len - 1; pos >= 0; pos--) {
            if (rev_idx >= MAX_WORD_LEN) {
                break;
            }
            reversed[rev_idx] = words[idx][pos];
            rev_idx++;
        }
        reversed[rev_idx] = '\0';
        
        printf("Original: %s, Reversed: %s\n", words[idx], reversed);
    }
    
    return 0;
}