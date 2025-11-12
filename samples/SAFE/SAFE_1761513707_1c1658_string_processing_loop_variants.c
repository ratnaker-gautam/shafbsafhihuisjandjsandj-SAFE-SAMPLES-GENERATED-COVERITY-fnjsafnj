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
        
        printf("Word %d: length=%zu", k + 1, word_len);
        if (has_upper) printf(", has uppercase");
        if (has_lower) printf(", has lowercase");
        if (has_digit) printf(", has digits");
        printf("\n");
        
        processed++;
    }
    
    printf("\nReversed words:\n");
    k = word_count - 1;
    while (k >= 0) {
        size_t word_len = strlen(words[k]);
        if (word_len > 0) {
            int start = 0;
            int end = word_len - 1;
            while (start < end) {
                char temp = words[k][start];
                words[k][start] = words[k][end];
                words[k][end] = temp;
                start++;
                end--;
            }
            printf("%s ", words[k]);
        }
        k--;
    }
    printf("\n");
    
    return 0;
}