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
        printf("%d: %s\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    int processed = 0;
    for (k = 0; k < word_count; k++) {
        char current[MAX_WORD_LEN + 1];
        strncpy(current, words[k], sizeof(current) - 1);
        current[sizeof(current) - 1] = '\0';
        
        int word_len = strlen(current);
        int has_alpha = 0;
        
        for (int m = 0; m < word_len; m++) {
            if (isalpha((unsigned char)current[m])) {
                has_alpha = 1;
                break;
            }
        }
        
        if (has_alpha) {
            int vowel_count = 0;
            int consonant_count = 0;
            
            for (int m = 0; m < word_len; m++) {
                char ch = tolower((unsigned char)current[m]);
                if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                    vowel_count++;
                } else if (isalpha((unsigned char)current[m])) {
                    consonant_count++;
                }
            }
            
            printf("Word '%s': %d vowels, %d consonants\n", current, vowel_count, consonant_count);
            processed++;
        } else {
            printf("Word '%s': no alphabetic characters\n", current);
        }
    }
    
    printf("\nTotal words processed: %d\n", processed);
    return 0;
}