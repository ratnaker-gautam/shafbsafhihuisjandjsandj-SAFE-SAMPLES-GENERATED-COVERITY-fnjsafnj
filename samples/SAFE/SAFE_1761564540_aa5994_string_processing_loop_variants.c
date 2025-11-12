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
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int j = 0;
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
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Found %d words:\n", word_count);
    for (int idx = 0; idx < word_count; idx++) {
        printf("%d: %s\n", idx + 1, words[idx]);
    }
    
    printf("\nProcessing words:\n");
    int k = 0;
    do {
        int word_len = strlen(words[k]);
        int vowel_count = 0;
        int consonant_count = 0;
        
        for (int m = 0; m < word_len; m++) {
            char c = tolower((unsigned char)words[k][m]);
            if (c >= 'a' && c <= 'z') {
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
        }
        
        printf("Word '%s': %d vowels, %d consonants\n", words[k], vowel_count, consonant_count);
        k++;
    } while (k < word_count);
    
    printf("\nReversed words:\n");
    for (int idx = 0; idx < word_count; idx++) {
        int word_len = strlen(words[idx]);
        char reversed[MAX_WORD_LEN + 1];
        
        int n = word_len - 1;
        int p = 0;
        while (n >= 0) {
            reversed[p] = words[idx][n];
            p++;
            n--;
        }
        reversed[p] = '\0';
        
        printf("%s -> %s\n", words[idx], reversed);
    }
    
    return 0;
}