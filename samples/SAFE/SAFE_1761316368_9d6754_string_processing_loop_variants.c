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
        while (i < len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
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
        if (word_len == 0) {
            continue;
        }
        
        int has_digit = 0;
        int has_upper = 0;
        int has_lower = 0;
        
        for (j = 0; j < word_len; j++) {
            unsigned char c = words[k][j];
            if (isdigit(c)) {
                has_digit = 1;
            } else if (isupper(c)) {
                has_upper = 1;
            } else if (islower(c)) {
                has_lower = 1;
            }
        }
        
        printf("Word '%s': ", words[k]);
        if (has_digit) {
            printf("contains digits, ");
        }
        if (has_upper) {
            printf("has uppercase, ");
        }
        if (has_lower) {
            printf("has lowercase, ");
        }
        if (!has_digit && !has_upper && !has_lower) {
            printf("only punctuation, ");
        }
        
        int vowel_count = 0;
        int consonant_count = 0;
        for (j = 0; j < word_len; j++) {
            char c = tolower((unsigned char)words[k][j]);
            if (c >= 'a' && c <= 'z') {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
        }
        printf("vowels=%d, consonants=%d\n", vowel_count, consonant_count);
        processed++;
    }
    
    printf("\nProcessed %d words successfully\n", processed);
    return 0;
}