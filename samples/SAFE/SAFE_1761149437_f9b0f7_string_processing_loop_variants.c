//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN + 1];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input\n");
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
        
        if (i >= len) break;
        
        j = i;
        while (j < len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len > 0 && word_len <= MAX_INPUT_LEN) {
            strncpy(words[word_count], input + i, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        i = j;
    }
    
    printf("Found %d words:\n", word_count);
    for (k = 0; k < word_count; k++) {
        printf("%d: %s\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        int word_len = strlen(words[k]);
        if (word_len > 0) {
            int has_upper = 0;
            int has_lower = 0;
            int has_digit = 0;
            
            char *ptr = words[k];
            do {
                if (isupper((unsigned char)*ptr)) has_upper = 1;
                if (islower((unsigned char)*ptr)) has_lower = 1;
                if (isdigit((unsigned char)*ptr)) has_digit = 1;
                ptr++;
            } while (*ptr != '\0');
            
            printf("Word '%s': ", words[k]);
            if (has_upper) printf("UPPER ");
            if (has_lower) printf("LOWER ");
            if (has_digit) printf("DIGIT ");
            printf("\n");
            
            processed++;
        }
    }
    
    printf("\nReversed words:\n");
    k = word_count - 1;
    while (k >= 0) {
        int word_len = strlen(words[k]);
        if (word_len > 0) {
            for (int pos = word_len - 1; pos >= 0; pos--) {
                putchar(words[k][pos]);
            }
            putchar(' ');
        }
        k--;
    }
    putchar('\n');
    
    return 0;
}