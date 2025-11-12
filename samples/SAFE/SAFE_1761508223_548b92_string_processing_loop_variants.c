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
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("Original words:\n");
    for (k = 0; k < word_count; k++) {
        printf("%d: %s\n", k + 1, words[k]);
    }
    
    printf("\nProcessing words:\n");
    
    for (k = 0; k < word_count; k++) {
        char temp[MAX_WORD_LEN + 1];
        size_t word_len = strlen(words[k]);
        
        if (word_len == 0) continue;
        
        int m = 0;
        int n = word_len - 1;
        while (m <= n) {
            temp[m] = words[k][n];
            temp[n] = words[k][m];
            m++;
            n--;
        }
        temp[word_len] = '\0';
        
        int vowel_count = 0;
        int p = 0;
        do {
            char ch = tolower((unsigned char)temp[p]);
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                vowel_count++;
            }
            p++;
        } while (temp[p] != '\0');
        
        int digit_count = 0;
        int q = 0;
        while (temp[q] != '\0') {
            if (isdigit((unsigned char)temp[q])) {
                digit_count++;
            }
            q++;
        }
        
        printf("Word %d: '%s' -> '%s' (vowels: %d, digits: %d)\n", 
               k + 1, words[k], temp, vowel_count, digit_count);
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int total_vowels = 0;
    int total_digits = 0;
    
    for (k = 0; k < word_count; k++) {
        size_t word_len = strlen(words[k]);
        total_chars += word_len;
        
        int r = 0;
        for (r = 0; words[k][r] != '\0'; r++) {
            char ch = tolower((unsigned char)words[k][r]);
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                total_vowels++;
            }
            if (isdigit((unsigned char)words[k][r])) {
                total_digits++;
            }
        }
    }
    
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", total_chars);
    printf("Total vowels: %d\n", total_vowels);
    printf("Total digits: %d\n", total_digits);
    
    return 0;
}