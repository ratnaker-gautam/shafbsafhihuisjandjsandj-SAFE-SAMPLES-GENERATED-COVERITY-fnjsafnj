//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text: ");
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
        return 1;
    }
    
    if (len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < len) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = i;
        while (j < len && !isspace(input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len > 0 && word_len < 64) {
            if (word_count >= MAX_WORDS) {
                fprintf(stderr, "Too many words\n");
                return 1;
            }
            
            for (k = 0; k < word_len; k++) {
                words[word_count][k] = input[i + k];
            }
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Processing %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        size_t current_len = strlen(words[i]);
        for (j = 0; j < current_len; j++) {
            if (isupper(words[i][j])) {
                has_upper = 1;
            } else if (islower(words[i][j])) {
                has_lower = 1;
            } else if (isdigit(words[i][j])) {
                has_digit = 1;
            }
        }
        
        printf("Word %d: \"%s\" (len: %zu)", i + 1, words[i], current_len);
        
        if (has_upper && has_lower && has_digit) {
            printf(" - Mixed case with digits\n");
        } else if (has_upper && has_lower) {
            printf(" - Mixed case\n");
        } else if (has_upper) {
            printf(" - All uppercase\n");
        } else if (has_lower) {
            printf(" - All lowercase\n");
        } else if (has_digit) {
            printf(" - Numbers only\n");
        } else {
            printf(" - Other characters\n");
        }
    }
    
    int total_chars = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters in words: %d\n", total_chars);
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    
    return 0;
}