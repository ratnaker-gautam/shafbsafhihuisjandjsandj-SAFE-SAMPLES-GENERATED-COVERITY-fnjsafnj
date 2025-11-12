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
    
    i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        j = 0;
        while (i < len && !isspace((unsigned char)input[i]) && j < 63) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        word_count++;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Processing words:\n");
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        int vowel_count = 0;
        int consonant_count = 0;
        
        for (i = 0; i < word_len; i++) {
            char c = tolower((unsigned char)words[k][i]);
            if (c >= 'a' && c <= 'z') {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
        }
        
        printf("Word %d: '%s' (length: %zu, vowels: %d, consonants: %d)\n", 
               k + 1, words[k], word_len, vowel_count, consonant_count);
    }
    
    printf("\nReversed words:\n");
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        char reversed[64];
        
        if (word_len > 63) {
            fprintf(stderr, "Word too long for reversal\n");
            return 1;
        }
        
        for (i = 0, j = word_len - 1; i < word_len; i++, j--) {
            reversed[i] = words[k][j];
        }
        reversed[word_len] = '\0';
        
        printf("%s ", reversed);
    }
    printf("\n");
    
    return 0;
}