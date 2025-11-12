//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT];
    char words[MAX_WORDS][64];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int i = 0;
    while (i < len && word_count < MAX_WORDS) {
        while (i < len && isspace(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && !isspace(input[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len < 64) {
            strncpy(words[word_count], &input[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    printf("Words found: %d\n", word_count);
    
    for (int idx = 0; idx < word_count; idx++) {
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        char *ptr = words[idx];
        while (*ptr) {
            if (isupper(*ptr)) has_upper = 1;
            if (islower(*ptr)) has_lower = 1;
            if (isdigit(*ptr)) has_digit = 1;
            ptr++;
        }
        
        printf("Word %d: \"%s\" (", idx + 1, words[idx]);
        if (has_upper) printf("U");
        if (has_lower) printf("L");
        if (has_digit) printf("D");
        printf(")\n");
    }
    
    int total_chars = 0;
    int j = 0;
    do {
        if (j < word_count) {
            total_chars += strlen(words[j]);
        }
        j++;
    } while (j < word_count);
    
    printf("Total characters in words: %d\n", total_chars);
    
    int palindrome_count = 0;
    for (int k = 0; k < word_count; k++) {
        int is_pal = 1;
        size_t wlen = strlen(words[k]);
        for (size_t m = 0; m < wlen / 2; m++) {
            if (tolower(words[k][m]) != tolower(words[k][wlen - 1 - m])) {
                is_pal = 0;
                break;
            }
        }
        if (is_pal && wlen > 0) {
            palindrome_count++;
        }
    }
    
    printf("Palindromes found: %d\n", palindrome_count);
    
    return 0;
}