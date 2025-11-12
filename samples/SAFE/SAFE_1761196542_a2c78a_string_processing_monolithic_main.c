//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = i;
        while (j < input_len && !isspace((unsigned char)input[j])) {
            j++;
        }
        
        int word_len = j - i;
        if (word_len >= 64) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        strncpy(words[word_count], &input[i], word_len);
        words[word_count][word_len] = '\0';
        
        for (k = 0; k < word_len; k++) {
            words[word_count][k] = tolower((unsigned char)words[word_count][k]);
        }
        
        word_count++;
        i = j;
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Processing %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        int is_palindrome = 1;
        int len = strlen(words[i]);
        
        for (j = 0; j < len / 2; j++) {
            if (words[i][j] != words[i][len - 1 - j]) {
                is_palindrome = 0;
                break;
            }
        }
        
        int vowel_count = 0;
        for (j = 0; j < len; j++) {
            char c = words[i][j];
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            }
        }
        
        printf("Word %d: \"%s\" (length: %d, vowels: %d, %s)\n", 
               i + 1, words[i], len, vowel_count, 
               is_palindrome ? "palindrome" : "not palindrome");
    }
    
    int total_chars = 0;
    int total_vowels = 0;
    for (i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
        for (j = 0; j < strlen(words[i]); j++) {
            char c = words[i][j];
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                total_vowels++;
            }
        }
    }
    
    printf("\nSummary: %d words, %d total characters, %d vowels\n", 
           word_count, total_chars, total_vowels);
    
    return 0;
}