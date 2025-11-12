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
    char temp[MAX_WORD_LEN + 1];
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
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            temp[j++] = input[i++];
        }
        temp[j] = '\0';
        
        if (j > 0) {
            strncpy(words[word_count], temp, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
    }
    
    printf("Found %d words:\n", word_count);
    for (k = 0; k < word_count; k++) {
        printf("%d: %s\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        int has_digit = 0;
        
        j = 0;
        do {
            if (isdigit((unsigned char)words[k][j])) {
                has_digit = 1;
                break;
            }
            j++;
        } while (j < word_len);
        
        if (!has_digit) {
            char reversed[MAX_WORD_LEN + 1];
            int m = word_len - 1;
            int n = 0;
            
            while (m >= 0 && n < MAX_WORD_LEN) {
                reversed[n++] = words[k][m--];
            }
            reversed[n] = '\0';
            
            printf("Reversed '%s' -> '%s'\n", words[k], reversed);
            processed++;
        } else {
            printf("Skipped '%s' (contains digits)\n", words[k]);
        }
    }
    
    printf("\nTotal words processed: %d\n", processed);
    return 0;
}