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
        printf("%d: '%s'\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        int has_digit = 0;
        
        for (j = 0; j < word_len; j++) {
            if (isdigit(words[k][j])) {
                has_digit = 1;
                break;
            }
        }
        
        if (!has_digit) {
            for (j = 0; j < word_len / 2; j++) {
                char temp = words[k][j];
                words[k][j] = words[k][word_len - 1 - j];
                words[k][word_len - 1 - j] = temp;
            }
            printf("Reversed '%s'\n", words[k]);
            processed++;
        } else {
            printf("Skipped '%s' (contains digits)\n", words[k]);
        }
    }
    
    printf("\nFinal word list:\n");
    k = 0;
    do {
        if (word_count > 0) {
            printf("%d: '%s'\n", k + 1, words[k]);
        }
        k++;
    } while (k < word_count);
    
    return 0;
}