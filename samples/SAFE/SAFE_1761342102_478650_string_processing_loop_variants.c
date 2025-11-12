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
    
    printf("Enter a string: ");
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
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Words found: %d\n", word_count);
    
    for (k = 0; k < word_count; k++) {
        printf("Word %d: ", k + 1);
        
        int m = 0;
        do {
            if (m < (int)strlen(words[k])) {
                putchar(words[k][m]);
                m++;
            } else {
                break;
            }
        } while (1);
        
        printf(" (length: %zu)\n", strlen(words[k]));
    }
    
    printf("\nProcessing words:\n");
    
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        
        if (word_len == 0) {
            continue;
        }
        
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        char *ptr = words[k];
        while (*ptr != '\0') {
            if (isupper((unsigned char)*ptr)) {
                has_upper = 1;
            } else if (islower((unsigned char)*ptr)) {
                has_lower = 1;
            } else if (isdigit((unsigned char)*ptr)) {
                has_digit = 1;
            }
            ptr++;
        }
        
        printf("Word '%s': ", words[k]);
        
        if (has_upper && has_lower && has_digit) {
            printf("mixed case with digits");
        } else if (has_upper && has_lower) {
            printf("mixed case");
        } else if (has_upper) {
            printf("all uppercase");
        } else if (has_lower) {
            printf("all lowercase");
        } else if (has_digit) {
            printf("all digits");
        } else {
            printf("other characters");
        }
        
        printf("\n");
        processed++;
    }
    
    printf("Total words processed: %d\n", processed);
    
    return 0;
}